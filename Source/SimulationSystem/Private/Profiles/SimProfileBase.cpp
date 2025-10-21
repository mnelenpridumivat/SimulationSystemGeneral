// Fill out your copyright notice in the Description page of Project Settings.


#include "Profiles/SimProfileBase.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "NavigationSystem.h"
#include "ProfileComponent.h"
#include "ProfileIDController.h"
#include "SimActorInterface.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemSettings.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "DEPRECATED_ReplicatedSimInfo.h"
#include "GlobalGraph.h"
#include "GraphAsset.h"
#include "GraphSerialized.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Net/UnrealNetwork.h"

void USimProfileBase::NativeOnVertexPositionChanged(const FSimVertexID& Old,
	const FSimVertexID& New)
{
	OnVertexPositionChanged.Broadcast(this, Old, New);
}

void USimProfileBase::NativeSave(FSimVertexID VertexID, FSerializedProfileView Data)
{
	Data.GetElem().ObjectClass = GetClass();
	Data.GetElem().VertexLocation = VertexID;
	USimulationFunctionLibrary::SaveObjectData(this, Data.GetElem().ObjectData);
	if(HasOnlineActor() && GetSimLevel() == ESimulationLevels::Online)
	{
		auto Actor = GetOnlineActor();
		if(ensure(Actor))
		{
			Data.GetElem().NextSet();
			auto OnlineDataSingle = Data.GetElem().AddChild();
			OnlineDataSingle.GetElem().VertexLocation = VertexID;
			OnlineDataSingle.GetElem().ObjectClass = Actor->GetClass();
			USimulationFunctionLibrary::SaveObjectData(Actor, OnlineDataSingle.GetElem().ObjectData);
		}
	}
	Save(VertexID, Data);
}

void USimProfileBase::NativeLoad(FSerializedProfile& Data)
{
	if(HasOnlineActor() && GetSimLevel() == ESimulationLevels::Online)
	{
		FProfilesSerializedView OnlineData;
		Data.ExtractFirstChildren(OnlineData);
		if(const auto ActorData = OnlineData.Objects[0];
			ensure(ActorData->ObjectClass->IsChildOf(AActor::StaticClass())))
		{
			static FActorSpawnParameters SpawnParams;
			SpawnParams.bNoFail = true;
			auto NewActor = GetWorld()->SpawnActor(ActorData->ObjectClass, nullptr, nullptr, SpawnParams);
			USimulationFunctionLibrary::LoadObjectData(NewActor, ActorData->ObjectData);
		}
	}
	Load(Data);
}

void USimProfileBase::NativeOnRegistered()
{
	OnRegistered();
	auto LocalGraphInside = USimulationFunctionLibrary::GetGraphAssetFromProfile(GetWorld(), this);
	if (IsValid(LocalGraphInside))
	{
		auto Registry = LocalGraphInside->GetRegistry();
		if (ensureMsgf(
			IsValid(Registry),
			TEXT("GraphAsset [%s] does not contains valid Registry!"),
			*LocalGraphInside->GetName()))
		{
			Registry->NativeRegisterProfile(this);
		}
	}
}

bool USimProfileBase::IsMovable_Implementation()
{
	return true;
}

USimProfileBase::USimProfileBase()
{
	ProfileID = UProfileIDController::InvalidID;
}

void USimProfileBase::NativeTick(float DeltaTime)
{
	Tick(DeltaTime);
}

void USimProfileBase::NativeOnPostRegistered()
{
	OnPostRegistered();
}

void USimProfileBase::NativeOnCreated()
{
	OnCreated();
}

void USimProfileBase::NativeOnLoaded()
{
	OnLoaded();
}

USimProfileBase* USimProfileBase::DeepCopyProfile()
{
	auto Copy = DuplicateObject(this, GetWorld());
	Copy->OnCreated();
	return Copy;
}

void USimProfileBase::TransitOfflineBuffered()
{
}

void USimProfileBase::TransitBufferedOffline()
{
}

void USimProfileBase::TransitBufferedOnline()
{
	if(OnlineActorClass.IsNull())
	{
		return;
	}
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	StreamableHandle = Streamable.RequestAsyncLoad(
		OnlineActorClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(
			this,
			&USimProfileBase::OnOnlineActorClassLoaded
			));
}

void USimProfileBase::TransitOnlineBuffered()
{
	if(StreamableHandle.IsValid())
	{
		StreamableHandle.Get()->CancelHandle();
		StreamableHandle.Reset();
	}
	if(IsValid(OnlineActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Destroying online actor %s for profile %s!"), *OnlineActor->GetName(), *GetName());
		OnlineActor->Destroy();
	}
}

void USimProfileBase::OnOnlineActorClassLoaded()
{
	FNavLocation ProjectedPoint(OnlineLocation.GetLocation());
	// TODO: Add switcher if force spawn on navmesh
	FVector SpawnLocation = ProjectedPoint.Location;
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	OnlineActor = GetWorld()->SpawnActor(OnlineActorClass.Get(), &SpawnLocation, nullptr, params);
	if(!OnlineActor->GetClass()->ImplementsInterface(USimActorInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn actor from profile without ISimActorInterface!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Online actor %s for profile %s spawned!"), *OnlineActor->GetName(), *GetName());
	ISimActorInterface::Execute_GetProfileComponent(OnlineActor)->SetProfileID(ProfileID);
}

void USimProfileBase::SetOnlineLocation(FVector Vector)
{
	OnlineLocation.SetLocation(Vector);
}

FTransform USimProfileBase::GetOnlineLocation() const
{
	if(IsValid(OnlineActor))
	{
		return OnlineActor->GetActorTransform();
	}
	return OnlineLocation;
}

void USimProfileBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USimProfileBase, OnlineLocation);
	DOREPLIFETIME(USimProfileBase, ProfileID);
	DOREPLIFETIME(USimProfileBase, CurrentSimLevel);
	DOREPLIFETIME(USimProfileBase, OnlineActor);
}

void USimProfileBase::GetDebugData(FDebugDataMain& Data)
{
	Data.Elems.Empty();
	for (auto PropIt : TFieldRange<FProperty>(GetClass()))
	{
		AddPropFunc({NAME_None,PropIt, this}, Data.Elems);
	}
}

void USimProfileBase::AddPropFunc(DebugDataPropertyHandle Prop, TArray<DebugDataElemBase*>& Arr)
{
	if (Prop.Type->IsA(FArrayProperty::StaticClass()))
	{
		AddArrayPropFunc(Prop, Arr);
	}
	else if (Prop.Type->IsA(FSetProperty::StaticClass()))
	{
		AddSetPropFunc(Prop, Arr);
	}
	else if (Prop.Type->IsA(FMapProperty::StaticClass()))
	{
		AddMapPropFunc(Prop, Arr);
	}
	else if (Prop.Type->IsA(FStructProperty::StaticClass()))
	{
		AddStructPropFunc(Prop, Arr);
	}
	else
	{
		AddSinglePropFunc(Prop, Arr);
	}
}

void USimProfileBase::AddArrayPropFunc(DebugDataPropertyHandle Property, TArray<DebugDataElemBase*>& Arr)
{
	auto Ptr = new DebugDataElemNested();
	Ptr->key = Property.GetName();

	auto CastedType = CastField<FArrayProperty>(Property.Type);
	FScriptArrayHelper ArrayHelper(CastedType, Property.Context);
	
	auto ArrayTypeProp = CastedType->Inner;
	for (SIZE_T i = 0; i < ArrayHelper.Num(); ++i)
	{
		auto ElemValue = ArrayHelper.GetElementPtr(i);
		AddPropFunc({*FString::FromInt(i), ArrayTypeProp, ElemValue}, Ptr->values);
	}

	Arr.Add(Ptr);
}

void USimProfileBase::AddSetPropFunc(DebugDataPropertyHandle Property, TArray<DebugDataElemBase*>& Arr)
{
	auto Ptr = new DebugDataElemNested();
	Ptr->key = Property.GetName();

	auto CastedType = CastField<FSetProperty>(Property.Type);
	FScriptSetHelper ArrayHelper(CastedType, Property.Context);

	auto SetTypeProp = CastedType->ElementProp;
	for (SIZE_T i = 0; i < ArrayHelper.Num(); ++i)
	{
		auto ElemValue = ArrayHelper.GetElementPtr(i);
		AddPropFunc({*FString::FromInt(i), SetTypeProp, ElemValue}, Ptr->values);
	}

	Arr.Add(Ptr);
}

void USimProfileBase::AddMapPropFunc(DebugDataPropertyHandle Property, TArray<DebugDataElemBase*>& Arr)
{
	auto Ptr = new DebugDataElemNested();
	Ptr->key = Property.GetName();

	auto CastedType = CastField<FMapProperty>(Property.Type);
	FScriptMapHelper helper(CastedType, Property.Context);
	
	for (SIZE_T i = 0; i < helper.Num(); ++i)
	{
		auto elemPtr = new DebugDataElemNested();
		
		void* KeyPtr = helper.GetKeyPtr(i);
		AddPropFunc({"Key", helper.KeyProp, KeyPtr}, elemPtr->values);
		void* ValuePtr = helper.GetValuePtr(i);
		AddPropFunc({"Key", helper.ValueProp, ValuePtr}, elemPtr->values);

		Arr.Add(elemPtr);
	}

	Arr.Add(Ptr);
}

void USimProfileBase::AddSinglePropFunc(DebugDataPropertyHandle Property, TArray<DebugDataElemBase*>& Arr)
{
	if (Property.Type->IsA(FObjectProperty::StaticClass()))
	{
		auto Casted = CastField<FObjectProperty>(Property.Type);
		auto Ptr = new DebugDataElemKeyObject();
		Ptr->key = Property.GetName();
		Ptr->object = Casted->GetObjectPropertyValue(Casted->ContainerPtrToValuePtr<void>(Property.Context));
		/*if (IsValid(Ptr->object))
		{
			Ptr->object->GetFName();
		}*/
		Arr.Add(Ptr);
	} 
	else if (Property.Type->IsA(FSoftObjectProperty::StaticClass()))
	{
		auto Casted = CastField<FSoftObjectProperty>(Property.Type);
		auto Ptr = new DebugDataElemKeyObject();
		Ptr->key = Property.GetName();
		Ptr->object = Casted->GetObjectPropertyValue(Casted->ContainerPtrToValuePtr<void>(Property.Context));
		/*if (IsValid(Ptr->object))
		{
			Ptr->object->GetFName();
		}*/
		Arr.Add(Ptr);
	} else if (Property.Type->IsA(FMulticastInlineDelegateProperty::StaticClass()))
	{
		auto Casted = CastField<FMulticastInlineDelegateProperty>(Property.Type);
		auto Ptr = new DebugDataElemKeyValue();
		Ptr->key = Property.GetName();
		Ptr->value = "Dynamic Multicast Delegate";
		Arr.Add(Ptr);
	}
	else
	{
		auto Ptr = new DebugDataElemKeyValue();
		Ptr->key = Property.GetName();
		Ptr->value = "Invalid";
		if (Property.Type->IsA(FStrProperty::StaticClass()))
		{
			auto Casted = CastField<FStrProperty>(Property.Type);
			Ptr->value = *Casted->ContainerPtrToValuePtr<FString>(Property.Context);
		}
		else if (Property.Type->IsA(FNameProperty::StaticClass()))
		{
			auto Casted = CastField<FNameProperty>(Property.Type);
			Ptr->value = Casted->ContainerPtrToValuePtr<FName>(Property.Context)->ToString();
		}
		else if (Property.Type->IsA(FBoolProperty::StaticClass()))
		{
			auto Casted = CastField<FBoolProperty>(Property.Type);
			Ptr->value = UKismetStringLibrary::Conv_BoolToString(*Casted->ContainerPtrToValuePtr<bool>(Property.Context));
		}
		else if (Property.Type->IsA(FNumericProperty::StaticClass()))
		{
			if (auto EnumCasted = CastField<FByteProperty>(Property.Type))
			{
				auto Enum = EnumCasted->GetIntPropertyEnum();
				Ptr->value = Enum->GetNameByValue(*EnumCasted->ContainerPtrToValuePtr<int64>(Property.Context)).ToString();
			} else
			{
				auto Casted = CastField<FNumericProperty>(Property.Type);
				Ptr->value = Casted->GetNumericPropertyValueToString(Casted->ContainerPtrToValuePtr<void>(Property.Context));
			}
		} else
		{
			checkf(false, TEXT("Unsupported property type [%s] for property [%s] in [%s]"), *Property.Type->GetClass()->GetName(), *Property.GetName().ToString(), *GetName());
		}
		Arr.Add(Ptr);
	}
}

void USimProfileBase::AddStructPropFunc(DebugDataPropertyHandle Property, TArray<DebugDataElemBase*>& Arr)
{
	auto Ptr = new DebugDataElemNested();
	Ptr->key = Property.GetName();

	auto Casted = CastField<FStructProperty>(Property.Type);
	for (auto PropIt : TFieldRange<FProperty>(Casted->Struct))
	{
		AddPropFunc({NAME_None, PropIt, Casted->ContainerPtrToValuePtr<void>(Property.Context)}, Ptr->values);
	}
		
	Arr.Add(Ptr);
}

void USimProfileBase::OnExit_Implementation(const FSimProfileHolder& Location)
{
	bool Success;
	USimProfileBase* Parent = nullptr;
	USimulationFunctionLibrary::AsProfile(Location, Parent, Success);
	if(Success)
	{
		ISimProfileContainer::Execute_RemoveItem(Parent, this);
	}
}

void USimProfileBase::OnEnter_Implementation(const FSimProfileHolder& Location)
{
	
	bool Success;
	USimProfileBase* Parent = nullptr;
	USimulationFunctionLibrary::AsProfile(Location, Parent, Success);
	if(Success)
	{
		ISimProfileContainer::Execute_AddItem(Parent, this);
	}
}

/*void USimProfileBase::SetupReplicatedData(ADEPRECATED_ReplicatedSimInfo* ReplicatedInfo) const
{
	ReplicatedInfo->SetProfileID(ProfileID);
}*/
