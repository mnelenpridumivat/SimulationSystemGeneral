// Fill out your copyright notice in the Description page of Project Settings.


#include "Profiles/SimProfileItemContainer.h"

#include "GlobalGraph.h"
#include "ProfileComponent.h"
#include "DEPRECATED_ReplicatedSimInfo.h"
#include "GraphSerialized.h"
#include "SimActorInterface.h"
#include "SimulationFunctionLibrary.h"

USimProfileBase* USimProfileItemContainer::DeepCopyProfile()
{
	auto Copy = Cast<USimProfileItemContainer>(Super::DeepCopyProfile());
	for(int i = 0; i < Copy->StoredItems.Num(); ++i)
	{
		Copy->StoredItems[i] = Cast<USimProfileItem>(Copy->StoredItems[i]->DeepCopyProfile());
	}
	return Copy;
}

void USimProfileItemContainer::AddItem_Implementation(USimProfileBase* Profile)
{
	ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Unable to put profile [%s] in profile [%s]"), *Profile->GetName(), *GetName());
	StoredItems.Add(Cast<USimProfileItem>(Profile));
	/*if(IsValid(OnlineActor))
	{
		ensureMsgf(OnlineActor->GetClass()->ImplementsInterface(USimActorInterface::StaticClass()),
		           TEXT("Attempt to get profile from actor [%s], which doesn't implements SimActorInterface!"),
		           *OnlineActor->GetName());
		ISimActorInterface::Execute_GetProfileComponent(OnlineActor)->GetReplicatedInfo()->AddChild(Profile);
	}*/
}

TArray<USimProfileBase*> USimProfileItemContainer::GetAllItems_Implementation()
{
	TArray<USimProfileBase*> OutArray;
	OutArray.Reserve(StoredItems.Num());
	for(auto& elem : StoredItems)
	{
		OutArray.Add(elem);
	}
	return OutArray;
}

bool USimProfileItemContainer::HasItem_Implementation(USimProfileBase* Profile)
{
	ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Profile [%s] cannot contain profile [%s] because cannot contain such class"), *GetName(), *Profile->GetName());
	return StoredItems.Contains(Cast<USimProfileItem>(Profile));
}

void USimProfileItemContainer::RemoveItem_Implementation(USimProfileBase* Profile)
{
	ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Profile [%s] cannot contain profile [%s] because cannot contain such class"), *GetName(), *Profile->GetName());
	StoredItems.Remove(Cast<USimProfileItem>(Profile));
}

bool USimProfileItemContainer::CanStoreItem_Implementation(USimProfileBase* Profile)
{
	return Profile->IsA(USimProfileItem::StaticClass());
}

void USimProfileItemContainer::NativeOnRegistered()
{
	Super::NativeOnRegistered();
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	for(auto& Item : StoredItems)
	{
		GlobalGraph->RegisterChildProfile(Item, this);
	}
}

void USimProfileItemContainer::SetOnlineLocation(FVector Vector)
{
	Super::SetOnlineLocation(Vector);
	for(auto& Item : StoredItems)
	{
		Item->SetOnlineLocation(Vector);
	}
}

void USimProfileItemContainer::NativeSave(FSimVertexID VertexID, FSerializedProfileView Data)
{
	Super::NativeSave(VertexID, Data);
	Data.GetElem().NextSet();
	for(auto Item : StoredItems)
	{
		Item->NativeSave(VertexID, Data.GetElem().AddChild());
	}
}

void USimProfileItemContainer::NativeLoad(FSerializedProfile& Data)
{
	Super::NativeLoad(Data);
	FProfilesSerializedView Children;
	Data.ExtractFirstChildren(Children);
	for(auto elem : Children.Objects)
	{
		auto Item = NewObject<USimProfileItem>(GetWorld(), elem->ObjectClass);
		Item->NativeLoad(*elem);
		Execute_AddItem(this, Item);
	}
}
