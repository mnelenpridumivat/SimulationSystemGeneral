// Fill out your copyright notice in the Description page of Project Settings.


#include "Profiles/SimProfileStash.h"

#include "GlobalGraph.h"
#include "GraphSerialized.h"
#include "SimulationFunctionLibrary.h"
#include "Profiles/SimProfileItem.h"

USimProfileBase* USimProfileStash::DeepCopyProfile()
{
	auto Copy = Cast<USimProfileStash>(Super::DeepCopyProfile());
	for(int i = 0; i < Copy->StoredItems.Num(); ++i)
	{
		Copy->StoredItems[i] = Cast<USimProfileItem>(Copy->StoredItems[i]->DeepCopyProfile());
	}
	return Copy;
}

void USimProfileStash::OnRegistered_Implementation()
{
	Super::OnRegistered_Implementation();
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	for(auto& Item : StoredItems)
	{
		GlobalGraph->RegisterChildProfile(Item, this);
	}
}

bool USimProfileStash::IsMovable_Implementation()
{
	return false;
}

void USimProfileStash::AddItem_Implementation(USimProfileBase* Profile)
{
	if (ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Unable to put profile [%s] in profile [%s]"), *Profile->GetName(), *GetName()))
	{
		StoredItems.Add(Cast<USimProfileItem>(Profile));
	}
}

TArray<USimProfileBase*> USimProfileStash::GetAllItems_Implementation()
{
	TArray<USimProfileBase*> OutArray;
	OutArray.Reserve(StoredItems.Num());
	for(auto& elem : StoredItems)
	{
		OutArray.Add(elem);
	}
	return OutArray;
}

bool USimProfileStash::HasItem_Implementation(USimProfileBase* Profile)
{
	if (!ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Profile [%s] cannot contain profile [%s] because cannot contain such class"), *GetName(), *Profile->GetName()))
	{
		return false;
	}
	return StoredItems.Contains(Cast<USimProfileItem>(Profile));
}

void USimProfileStash::RemoveItem_Implementation(USimProfileBase* Profile)
{
	if (ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Profile [%s] cannot contain profile [%s] because cannot contain such class"), *GetName(), *Profile->GetName()))
	{
		StoredItems.Remove(Cast<USimProfileItem>(Profile));
	}
}

bool USimProfileStash::CanStoreItem_Implementation(USimProfileBase* Profile)
{
	if (!ensure(IsValid(Profile)))
	{
		return false;
	}
	return Profile->IsA(USimProfileItem::StaticClass());
}

void USimProfileStash::Save_Implementation(FSimVertexID VertexID, FSerializedProfileView Data)
{
	Super::Save_Implementation(VertexID, Data);
	Data.GetElem().NextSet();
	for(auto Item : StoredItems)
	{
		Item->Save(VertexID, Data.GetElem().AddChild());
	}
}

void USimProfileStash::Load_Implementation(FSerializedProfile& Data)
{
	Super::Load_Implementation(Data);
	FProfilesSerializedView Children;
	Data.ExtractFirstChildren(Children);
	for(auto elem : Children.Objects)
	{
		auto Item = NewObject<USimProfileItem>(GetWorld(), elem->ObjectClass);
		Item->Load(*elem);
		Execute_AddItem(this, Item);
	}
}
