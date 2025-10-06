// Fill out your copyright notice in the Description page of Project Settings.


#include "Profiles/SimProfileCamp.h"

#include "GlobalGraph.h"
#include "GraphSerialized.h"
#include "SimulationFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Profiles/AISimProfileSquad.h"

USimProfileBase* USimProfileCamp::DeepCopyProfile()
{
	auto Copy = Cast<USimProfileCamp>(Super::DeepCopyProfile());
	for(int i = 0; i < Copy->Squads.Num(); ++i)
	{
		Copy->Squads[i] = Cast<UAISimProfileSquad>(Copy->Squads[i]->DeepCopyProfile());
	}
	return Copy;
}

void USimProfileCamp::AddItem_Implementation(USimProfileBase* Profile)
{
	if (!ensureMsgf(
		IsValid(Profile),
		TEXT("Attempt to add invalid profile in Camp [%s]"),
		*GetName()))
	{
		return;
	}
	if (ensureMsgf(
		Execute_CanStoreItem(this, Profile),
		TEXT("Unable to put profile [%s] in profile [%s]"),
		*Profile->GetName(),
		*GetName()))
	{
		auto CastedSquad = Cast<UAISimProfileSquad>(Profile);
		Squads.Add(CastedSquad);
		OnSquadEntered.Broadcast(this, CastedSquad);
	}
}

TArray<USimProfileBase*> USimProfileCamp::GetAllItems_Implementation()
{
	TArray<USimProfileBase*> OutArray;
	OutArray.Reserve(Squads.Num());
	for(auto& elem : Squads)
	{
		OutArray.Add(elem);
	}
	return OutArray;
}

bool USimProfileCamp::HasItem_Implementation(USimProfileBase* Profile)
{
	if (ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Profile [%s] cannot contain profile [%s] because cannot contain such class"), *GetName(), *Profile->GetName()))
	{
		return Squads.Contains(Cast<UAISimProfileSquad>(Profile));
	}
	return false;
}

void USimProfileCamp::RemoveItem_Implementation(USimProfileBase* Profile)
{
	if (ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Profile [%s] cannot contain profile [%s] because cannot contain such class"), *GetName(), *Profile->GetName()))
	{
		auto CastedSquad = Cast<UAISimProfileSquad>(Profile);
		Squads.Remove(CastedSquad);
		OnSquadExited.Broadcast(this, CastedSquad);
	}
}

bool USimProfileCamp::CanStoreItem_Implementation(USimProfileBase* Profile)
{
	if (ensureMsgf(
		IsValid(Profile),
		TEXT("Attempt to check compatibility with null inside [%s]!"),
		*GetName()))
	{
		return Profile->IsA(UAISimProfileSquad::StaticClass());
	}
	return false;
}

bool USimProfileCamp::IsMovable_Implementation()
{
	return false;
}

void USimProfileCamp::OnRegistered_Implementation()
{
	Super::OnRegistered_Implementation();
	
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	if (ensureMsgf((GlobalGraph != nullptr), TEXT("GlobalGraph cannot be null")))
	{
		for(auto& Squad : Squads)
		{
			GlobalGraph->RegisterChildProfile(Squad, this);
		}
	}
}

void USimProfileCamp::Save_Implementation(FSimVertexID VertexID, FSerializedProfileView Data)
{
	Super::Save_Implementation(VertexID, Data);
	Data.GetElem().NextSet();
	for(auto Squad : Squads)
	{
		Squad->Save(VertexID, Data.GetElem().AddChild());
	}
}

void USimProfileCamp::Load_Implementation(FSerializedProfile& Data)
{
	Super::Load_Implementation(Data);
	FProfilesSerializedView Children;
	Data.ExtractFirstChildren(Children);
	for(auto SquadData : Children.Objects)
	{
		auto Squad = NewObject<UAISimProfileSquad>(GetWorld(), SquadData->ObjectClass);
		Squad->Load(*SquadData);
		Execute_AddItem(this, Squad);
	}
}
