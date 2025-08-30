// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimProfileID.h"
#include "ProfileIDController.generated.h"

class ADEPRECATED_ReplicatedSimInfo;
class USimProfileBase;
//struct FSimProfileID;

USTRUCT()
struct FFreeProfileIDArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<uint32> FreeID;
	
};

USTRUCT()
struct FSimProfilesByID
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<uint32, USimProfileBase*> Profiles;
	
};

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UProfileIDController : public UObject
{
	GENERATED_BODY()

public:

	FSimProfileID RegisterProfile(USimProfileBase* SimProfileBase);

	UFUNCTION(BlueprintPure)
	USimProfileBase* GetProfile(const FSimProfileID& ID);
	TArray<USimProfileBase*> GetProfiles(TSubclassOf<USimProfileBase> Class);

	static const FSimProfileID InvalidID;// = {nullptr, static_cast<uint32>(-1)};

	void ClearAllProfiles(){ProfilesByID.Empty();MaxUsedIDByClass.Empty();FreeIDs.Empty();}

	//void AddReplicatedSimInfo(ADEPRECATED_ReplicatedSimInfo* ReplicatedSimInfo);
	//void RemoveReplicatedSimInfo(ADEPRECATED_ReplicatedSimInfo* ReplicatedSimInfo);

	//UFUNCTION(BlueprintPure)
	//ADEPRECATED_ReplicatedSimInfo* GetReplicatedSimInfo(FSimProfileID ProfileID);

protected:

	//UPROPERTY()
	//TMap<FSimProfileID, ADEPRECATED_ReplicatedSimInfo*> ReplicatedProfilesData; 

	UPROPERTY()
	TMap<TSubclassOf<USimProfileBase>, FSimProfilesByID> ProfilesByID;

	UPROPERTY()
	TMap<TSubclassOf<USimProfileBase>, uint32> MaxUsedIDByClass;

	UPROPERTY()
	TMap<TSubclassOf<USimProfileBase>, FFreeProfileIDArray> FreeIDs;
	
};
