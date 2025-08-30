// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSystemSubsystem.h"

#include "SimulationSystemFunctionsImplementation.h"
#include "SimulationSystemSettings.h"

void USimulationSystemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	auto Settings = GetDefault<USimulationSystemSettings>();
	if (ensureMsgf(!Settings->SimulationSystemFunctions.IsNull(), TEXT("Simulation system function not set!")))
	{
		SimulationSystemFunctions = NewObject<USimulationSystemFunctionsImplementation>(
			Settings->SimulationSystemFunctions.LoadSynchronous());
		SimulationSystemFunctions->PreprocessPawnData(ClassComposedData);
	}
}

USimProfileBase* USimulationSystemSubsystem::ExecuteGenerator(UObject* Context, const FGeneratorHandleBase& handle)
{
	if (!ensure(IsValid(Context)))
	{
		return nullptr;
	}
	auto Class = handle.GeneratorClass;

	FScopeLock Lock(&GeneratorMutex);
	UProfileGenerator* Generator = nullptr;
	if (auto It = Generators.Find(Class))
	{
		Generator = *It;
	} else
	{
		Generator = Generators.Add(Class, NewObject<UProfileGenerator>(Context, Class));
	}
	ensure(Generator);

	handle.SetupGenerator(Generator);
	return Generator->GenerateProfile();
}
