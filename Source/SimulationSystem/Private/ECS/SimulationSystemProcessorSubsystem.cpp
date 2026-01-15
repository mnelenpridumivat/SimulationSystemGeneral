// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSystemProcessorSubsystem.h"

void USimulationSystemProcessorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
#if !UE_BUILD_SHIPPING
	DebugProcessorInstance = NewObject<UDebugProcessor>();	
#endif
	GraphMoveToProcessorInstance = NewObject<UGraphMoveToProcessor>();
}

void USimulationSystemProcessorSubsystem::Deinitialize()
{
#if !UE_BUILD_SHIPPING
	DebugProcessorInstance = nullptr;
#endif
	GraphMoveToProcessorInstance = nullptr;
	Super::Deinitialize();
}
