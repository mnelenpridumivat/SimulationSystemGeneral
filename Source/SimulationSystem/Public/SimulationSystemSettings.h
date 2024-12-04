// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Properties\PawnProperty.h"
#include "Replication/ReplicatedSimInfo.h"
#include "UObject/Object.h"
#include "SimulationSystemSettings.generated.h"

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
#endif

private:
	GENERATED_BODY()

public:
	
	//UPROPERTY(config, EditAnywhere, Category="Classes")
	//TSubclassOf<UAISimProfileSquad> SquadProfileClass;
	
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
	
	//UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics", meta=(ClampMin="0"))
	//int CharacteristicLevelDefault = 5;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics", meta=(ClampMin="0"))
	float PointsLevelMin = 0;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics", meta=(ClampMin="1"))
	float PointsLevelMax = 100;
	
	//UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics", meta=(ClampMin="0"))
	//float PointsLevelDefault = 100;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics")
	TMap<FName, int> CharacteristicsList;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Characteristics")
	TMap<FName, float> PointsList;

	/*
	 * Limit for pathfinding distance
	 */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Pathfinding")
	float FindWayMaxSearchDistance = 100000;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="Pathfinding")
	TSubclassOf<AReplicatedSimInfo> ReplicatedSimInfoClass = AReplicatedSimInfo::StaticClass();

};
