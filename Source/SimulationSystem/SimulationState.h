// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphSerialized.h"
#include "SimProfileBase.h"
#include "GameFramework/SaveGame.h"
#include "SimulationState.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationState : public USaveGame
{
	GENERATED_BODY()
	
	UPROPERTY()
	FProfilesSerialized Profiles;

	//UPROPERTY()
	//TMap<FSimVertexID, USimProfileBase*> Profiles;

	void SaveProfile(USimProfileBase* Profile, /*uint32 ParentIndex, uint32& Counter, */FSimVertexID VertexID = FSimVertexID::Invalid);
	USimProfileBase* LoadProfile(int& Index);

public:

	TMap<FSimVertexID, USimProfileBase*> GetProfiles();
	void SetProfiles(const TMap<FSimVertexID, USimProfileBase*>& NewProfiles);
	
};
