// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Properties\PawnProperty.h"
#include "CommunityRelationTable.h"
#include "LocalGraphRegistry.h"
#include "SimulationSystemFunctionsImplementation.h"
#include "SimulationSystemProfileType.h"
#include "Replication/DEPRECATED_ReplicatedSimInfo.h"
#include "UObject/Object.h"
#include "SimulationSystemSettings.generated.h"

class UCommunityRelationTable;
class ULocalGraphRegistry;
class UAISimProfilePawn;
class USimulationSystemFunctionsImplementation;
class UAISimProfileSquad;

USTRUCT(BlueprintType)
struct FSimulationSystemEntityDataTableKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Key;

	FORCEINLINE bool operator==(const FSimulationSystemEntityDataTableKey& other) const{ return Key == other.Key; }
};


FORCEINLINE uint32 GetTypeHash(const FSimulationSystemEntityDataTableKey& Data)
{
	return GetTypeHash(Data.Key);
}

/**
 * 
 */
UCLASS(Config=Engine, defaultconfig, BlueprintType)
class SIMULATIONSYSTEM_API USimulationSystemSettings : public UObject
{
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	DECLARE_MULTICAST_DELEGATE(FOnPropertyChanged);
	FOnPropertyChanged OnFunctionsPropertyChanged;
	FOnPropertyChanged ReprocessNPCClassesDelegate;
	
#endif

private:
	GENERATED_BODY()

public:

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="General")
	ESimualtionSystemProfileType ProfileType = ESimualtionSystemProfileType::ECS;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="SimulationLevel")
	float OnlineRadius = 5000;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="SimulationLevel")
	float BufferedRadius = 7000;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="SimulationLevel")
	float TickOffline = 1.0f;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="SimulationLevel")
	float TickBuffered = 0.5f;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="SimulationLevel")
	float TickOnline = 0.1f;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics", meta=(ClampMin="0"))
	int CharacteristicLevelMin = 0;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics", meta=(ClampMin="1"))
	int CharacteristicLevelMax = 10;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics", meta=(ClampMin="0"))
	float PointsLevelMin = 0;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics", meta=(ClampMin="1"))
	float PointsLevelMax = 100;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics")
	TMap<FName, int> CharacteristicsList;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics")
	TMap<FName, float> PointsList;

	/*
	 * Limit for pathfinding distance
	 */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Pathfinding")
	float FindWayMaxSearchDistance = 100000;

protected:
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Data")
	TSet<FName> EntitiesTableKeys;

public:
	TSet<FName>& GetEntitiesTableKeys();

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Data")
	TSoftObjectPtr<UDataTable> AllEntities;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Data")
	TMap<FSimulationSystemEntityDataTableKey, TSoftObjectPtr<UDataTable>> EntitiesTables;

	static UDataTable* GetDataTableByKey(FName Key);

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Data")
	FString GraphDataPath = TEXT("/Game/GraphDatas");

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="System")
	TSoftClassPtr<USimulationSystemFunctionsImplementation> SimulationSystemFunctions;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="System")
	TSoftClassPtr<ULocalGraphRegistry> LocalGraphRegistryClass = ULocalGraphRegistry::StaticClass();

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="System")
	TSoftClassPtr<UCommunityRelationTable> CommunityRelationTableClass = UCommunityRelationTable::StaticClass();
};
