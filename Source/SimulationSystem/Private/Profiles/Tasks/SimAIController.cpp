// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\Profiles\Tasks\SimAIController.h"

#include "AISimProfileBase.h"
#include "GlobalGraph.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemSettings.h"

void USimAIController::AddProfile(UAISimProfileBase* NewProfile)
{
	Profiles.Add(NewProfile);
	// if player - add to players set
}

void USimAIController::RemoveProfile(UAISimProfileBase* RemovedProfile)
{
	Profiles.Remove(RemovedProfile);
	// if player - remove from players set
}

void USimAIController::Tick(float DeltaTime)
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	TArray<FVector> PlayerLocations;
	PlayerLocations.Reserve(Players.Num());
	for(auto& Player : Players)
	{
		PlayerLocations.Add(GlobalGraph->GetVertexLocationByID(GlobalGraph->GetProfileLocationOnGraph(Player)));
	}
	for(auto& elem : Profiles)
	{
		float Closest = -1;
		for(auto& Player : PlayerLocations)
		{
			float Dist = FVector::Dist(GlobalGraph->GetVertexLocationByID(GlobalGraph->GetProfileLocationOnGraph(elem.Key)), Player);
			if(Closest < 0 || Dist < Closest)
			{
				Closest = Dist;
			}
		}
		if(Closest <= GetMutableDefault<USimulationSystemSettings>()->OnlineRadius)
		{
			elem.Key->SetSimLevel(ESimulationLevels::Online);
		}
		switch (elem.Key->GetSimLevel())
		{
		case ESimulationLevels::Online:
			{
				elem.Value = 0;
				//TickOnline()
			}
		}
	}
}
