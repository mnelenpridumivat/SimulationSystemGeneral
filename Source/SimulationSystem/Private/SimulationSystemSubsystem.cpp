// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSystemSubsystem.h"

#include "GlobalGraph.h"
#include "ProfileIDController.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemFunctionsImplementation.h"
#include "SimulationSystemSettings.h"

void USimulationSystemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
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
#if WITH_EDITOR
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
#endif
}

USimProfileBase* USimulationSystemSubsystem::ExecuteGenerator(UObject* Context, const FGeneratorHandleBase& handle)
{
	FScopeLock Lock(&GeneratorMutex);
	return GeneratorPool.ExecuteGenerator(Context, handle);
}

void USimulationSystemSubsystem::GetAllPawnClasses(TArray<FName>& OutPawnClasses)
{
	ClassComposedData.GetKeys(OutPawnClasses);
}

USimProfileBase* FGeneratorPool::ExecuteGenerator(UObject* Context, const FGeneratorHandleBase& handle)
{
	if (!ensure(IsValid(Context)))
	{
		return nullptr;
	}
	//auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(Context);
	//if (!ensureMsgf(IsValid(GlobalGraph), TEXT("Cannot find GlobalGraph on level!")))
	//{
	//	return nullptr;
	//}
	auto Class = handle.GeneratorClass;

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
	auto NewProfile = Generator->GenerateProfile();
	//NewProfile->SetProfileID(GlobalGraph->GetProfileIDsController()->RegisterProfile(NewProfile));
	return NewProfile;
}

FName USimulationSystemSubsystem::GetRandomNPCOfClass(FName NPCClass)
{
	auto it = ClassComposedData.Find(NPCClass);
	return it ? it->Variations[FMath::RandRange(0, it->Variations.Num() - 1)] : NAME_None;
}
