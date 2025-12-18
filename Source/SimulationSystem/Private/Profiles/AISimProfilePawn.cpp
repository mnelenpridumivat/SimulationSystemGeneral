// Fill out your copyright notice in the Description page of Project Settings.


#include "Profiles/AISimProfilePawn.h"

#include "GlobalGraph.h"
#include "GraphSerialized.h"
#include "SimulationFunctionLibrary.h"
#include "Profiles/SimProfileItem.h"

USimProfileBase* UAISimProfilePawn::DeepCopyProfile()
{
	auto Copy = Cast<UAISimProfilePawn>(Super::DeepCopyProfile());
	for(int i = 0; i < Copy->Inventory.Num(); ++i)
	{
		Copy->Inventory[i] = Cast<USimProfileItem>(Copy->Inventory[i]->DeepCopyProfile());
	}
	return Copy;
}

void UAISimProfilePawn::AddItem_Implementation(USimProfileBase* Profile)
{
#ifndef NoSimEnsureMessage
	ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Unable to put profile [%s] in profile [%s]"), IsValid(Profile) ? *Profile->GetName() : *FString("null"), *GetName());
#endif
//#if WITH_EDITOR
	if(!Execute_CanStoreItem(this, Profile))
	{
		return;
	}
//#endif
	Inventory.Add(Cast<USimProfileItem>(Profile));
}

TArray<USimProfileBase*> UAISimProfilePawn::GetAllItems_Implementation()
{
	TArray<USimProfileBase*> OutArray;
	OutArray.Reserve(Inventory.Num());
	for(auto& elem : Inventory)
	{
		OutArray.Add(elem);
	}
	return OutArray;
}

bool UAISimProfilePawn::HasItem_Implementation(USimProfileBase* Profile)
{
	ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Profile [%s] cannot contain profile [%s] because cannot contain such class"), *GetName(), *Profile->GetName());
	return Inventory.Contains(Cast<USimProfileItem>(Profile));
}

void UAISimProfilePawn::RemoveItem_Implementation(USimProfileBase* Profile)
{
	ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Profile [%s] cannot contain profile [%s] because cannot contain such class"), *GetName(), *Profile->GetName());
	Inventory.Remove(Cast<USimProfileItem>(Profile));
}

bool UAISimProfilePawn::CanStoreItem_Implementation(USimProfileBase* Profile)
{
#ifndef NoSimEnsureMessage
	ensureMsgf(IsValid(Profile), TEXT("Attempt to check store possibility for invalid profile!"));
#endif
//#if WITH_EDITOR
	if(!IsValid(Profile))
	{
		return false;
	}
//#endif
	return Profile->IsA(USimProfileItem::StaticClass());
}

void UAISimProfilePawn::NativeOnRegistered()
{
	Super::NativeOnRegistered();
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	for(auto& Item : Inventory)
	{
		GlobalGraph->RegisterChildProfile(Item, this);
	}
}

void UAISimProfilePawn::SetOnlineLocation(FVector Vector)
{
	Super::SetOnlineLocation(Vector);
	for(auto& Item : Inventory)
	{
		Item->SetOnlineLocation(Vector);
	}
}

void UAISimProfilePawn::NativeSave(FSimVertexID VertexID, FSerializedProfileView Data)
{
	Super::NativeSave(VertexID, Data);
	Data.GetElem().NextSet();
	for(auto Item : Inventory)
	{
		Item->NativeSave(VertexID, Data.GetElem().AddChild());
	}
}

void UAISimProfilePawn::NativeLoad(FSerializedProfile& Data)
{
	Super::NativeLoad(Data);
	ensure(0);
	/*FProfilesSerializedView Children;
	Data.ExtractFirstChildren(Children);
	for(auto& elem : Children.Objects)
	{
		auto Item = NewObject<USimProfileItem>(GetWorld(), elem->ObjectClass);
		Item->NativeLoad(*elem);
		Execute_AddItem(this, Item);
	}*/
}

void UAISimProfilePawn::NativeOnVertexPositionChanged(const FSimVertexID& Old, const FSimVertexID& New)
{
	Super::NativeOnVertexPositionChanged(Old, New);
	for (const auto Item : Inventory)
	{
		if (ensureMsgf(
			IsValid(Item),
			TEXT("Profile [%s] contains invalid item in inventory!"),
			*GetName()))
		{
			Item->NativeOnVertexPositionChanged(Old, New);
		}
	}
}
