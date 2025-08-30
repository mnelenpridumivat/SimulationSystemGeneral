// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSystemEditorSubsystem.h"

#include "SimulationSystemSettings.h"

void USimulationSystemEditorSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USimulationSystemEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	auto Settings = GetMutableDefault<USimulationSystemSettings>();
	if (!Settings->SimulationSystemFunctions.IsNull())
	{
		SimulationSystemFunctions = NewObject<USimulationSystemFunctionsImplementation>(
			this,
			Settings->SimulationSystemFunctions.LoadSynchronous()
			);
		SimulationSystemFunctions->PreprocessPawnData(ClassComposedData);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Simulation system function not set!"));
	}
	Settings->OnFunctionsPropertyChanged.AddLambda([this, Settings]()
	{
		SimulationSystemFunctions = nullptr;
		if (!Settings->SimulationSystemFunctions.IsNull())
		{
			SimulationSystemFunctions = NewObject<USimulationSystemFunctionsImplementation>(
			this,
			Settings->SimulationSystemFunctions.LoadSynchronous()
			);
			SimulationSystemFunctions->PreprocessPawnData(ClassComposedData);
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("Simulation system function not set!"));
		}
	});
	Settings->ReprocessNPCClassesDelegate.AddLambda([this, Settings]()
	{
		ClassComposedData.Empty();
		if (!SimulationSystemFunctions)
		{
			if (!Settings->SimulationSystemFunctions.IsNull())
			{
				SimulationSystemFunctions = NewObject<USimulationSystemFunctionsImplementation>(
			this,
				Settings->SimulationSystemFunctions.LoadSynchronous()
				);
			} else
			{
				UE_LOG(LogTemp, Error, TEXT("Simulation system function not set!"));
				return;
			}
		}
		SimulationSystemFunctions->PreprocessPawnData(ClassComposedData);
	});
}

void USimulationSystemEditorSubsystem::GetAllPawnClasses(TArray<FName>& OutPawnClasses)
{
	ClassComposedData.GetKeys(OutPawnClasses);
}
