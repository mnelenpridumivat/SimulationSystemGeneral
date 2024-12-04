// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSystemDeveloperFunctionLibrary.h"

#include "GlobalGraph.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemDeveloperSettings.h"

void USimulationSystemDeveloperFunctionLibrary::DebugDrawGraph(UWorld* World, float ShowTime)
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(World);
	GlobalGraph->DrawGraph(
		GetMutableDefault<USimulationSystemDeveloperSettings>()->LineColor,
		ShowTime,
		GetMutableDefault<USimulationSystemDeveloperSettings>()->LineThickness);
}
