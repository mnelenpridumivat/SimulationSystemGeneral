// Fill out your copyright notice in the Description page of Project Settings.


#include "Profiles/SimProfileCamp.h"

#include "GlobalGraph.h"
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
	if (ensureMsgf(Execute_CanStoreItem(this, Profile), TEXT("Unable to put profile [%s] in profile [%s]"), *Profile->GetName(), *GetName()))
	{
		Squads.Add(Cast<UAISimProfileSquad>(Profile));
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
		Squads.Remove(Cast<UAISimProfileSquad>(Profile));
	}
}

bool USimProfileCamp::CanStoreItem_Implementation(USimProfileBase* Profile)
{
	if (ensureMsgf(IsValid(Profile), TEXT("Attempt to check compatibility with null!")))
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
