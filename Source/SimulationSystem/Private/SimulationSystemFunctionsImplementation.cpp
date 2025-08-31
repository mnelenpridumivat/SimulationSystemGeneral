// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSystemFunctionsImplementation.h"

#include "AISimProfilePawn.h"
#include "AISimProfileSquad.h"
#include "ProfileGeneratorSquad.h"
#include "SimulationSystemSettings.h"

UAISimProfilePawn* USimulationSystemFunctionsImplementation::SpawnPawn_Implementation(UObject* Outer, FName PawnName)
{
	ensureMsgf(false, TEXT("SpawnPawn not implemented!"));
	return nullptr;
}

void USimulationSystemFunctionsImplementation::PreprocessPawnData_Implementation(
	TMap<FName, FPawnsOfClass>& ComposedData)
{
	ensureMsgf(false, TEXT("PreprocessPawnData not implemented!"));
}
