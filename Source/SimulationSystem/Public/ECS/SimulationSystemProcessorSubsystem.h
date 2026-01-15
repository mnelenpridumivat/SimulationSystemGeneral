// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugProcessor.h"
#include "GraphMoveToProcessor.h"
#include "MassSimulationSubsystem.h"
#include "SimulationSystemProcessorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationSystemProcessorSubsystem : public UMassSimulationSubsystem
{
	GENERATED_BODY()
 
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
 
private:
	
	UPROPERTY(Transient)
	UDebugProcessor* DebugProcessorInstance;

	UPROPERTY()
	UGraphMoveToProcessor* GraphMoveToProcessorInstance;
};
