// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Properties\PawnProperty.h"
#include "SimulationSystemFunctionsImplementation.h"
#include "Replication/DEPRECATED_ReplicatedSimInfo.h"
#include "UObject/Object.h"
#include "SimulationSystemSettings.generated.h"

class UAISimProfilePawn;
class USimulationSystemFunctionsImplementation;
class UAISimProfileSquad;

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
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="SimulationLevel")
	float OnlineRadius = 5000;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="SimulationLevel")
	float BufferedRadius = 7000;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="SimulationLevel")
	float TickOffline = 0.5f;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="SimulationLevel")
	float TickBuffered = 0.1f;
	
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

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Data")
	TSoftObjectPtr<UDataTable> Squads;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="System")
	TSoftClassPtr<USimulationSystemFunctionsImplementation> SimulationSystemFunctions;
};
