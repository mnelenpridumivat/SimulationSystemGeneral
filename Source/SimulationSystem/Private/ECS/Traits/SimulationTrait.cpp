// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationTrait.h"

void USimulationTrait::SetupDefaultTableSettings_Implementation(FInstancedStruct& data)
{
	//ensureMsgf(false, TEXT("Need to override USimulationTableTrait::SetupDefaultTableSettings!"));
}

void USimulationTrait::SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity,
	const FInstancedStruct& OverrideData)
{
}
