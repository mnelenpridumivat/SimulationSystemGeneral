// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSystemSubsystem.h"

#include "GlobalGraph.h"
#include "MassEntityManager.h"
#include "MassEntitySubsystem.h"
#include "MassEntityTemplate.h"
#include "MassEntityUtils.h"
#include "ProfileIDController.h"
#include "SimulationArchetype.h"
#include "SimulationArchetypeSettings.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemFunctionsImplementation.h"
#include "SimulationSystemSettings.h"
#include "SimulationTableTrait.h"

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

FMassEntityHandle USimulationSystemSubsystem::SpawnProfile(UObject* Context, FSimulationArchetypeHandle handle)
{
	if (!ensure(IsValid(Context)) || !ensure(IsValid(Context->GetWorld())))
	{
		return FMassEntityHandle();
	}
	auto& World = *Context->GetWorld();
	auto& EntityManager = UE::Mass::Utils::GetEntityManagerChecked(World);

	auto Settings = GetDefault<USimulationSystemSettings>();
	if (!ensure(!Settings->AllEntities.IsNull()))
	{
		return FMassEntityHandle();
	}
	auto EntitiesTable = Settings->AllEntities.LoadSynchronous();
	check(EntitiesTable);
	auto Row = EntitiesTable->FindRow<FSimulationArchetypeSettings>(handle.Name, nullptr);
	if (!ensure(Row))
	{
		return FMassEntityHandle();
	}
	auto Archetype = Row->Archetype.LoadSynchronous();
	if (!ensure(Archetype))
	{
		return FMassEntityHandle();
	}

	auto EntityTemplate = Archetype->GetOrCreateEntityTemplate(World);
	auto NewEntity = EntityManager.CreateEntity(EntityTemplate.GetArchetype());

	for (auto Trait : Archetype->GetConfig().GetTraits())
	{
		if (auto Casted = Cast<USimulationTableTrait>(Trait);
			Casted && ensure(Row->Overrides.Contains(Casted->GetClass())))
		{
			Casted->SetupEntity(EntityManager, NewEntity, Row->Overrides[Casted->GetClass()]);
		}
	}
	
	return NewEntity;
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
	auto NewProfile = Generator->GenerateProfileOld();
	//NewProfile->SetProfileID(GlobalGraph->GetProfileIDsController()->RegisterProfile(NewProfile));
	return NewProfile;
}

FName USimulationSystemSubsystem::GetRandomNPCOfClass(FName NPCClass)
{
	auto it = ClassComposedData.Find(NPCClass);
	return it ? it->Variations[FMath::RandRange(0, it->Variations.Num() - 1)] : NAME_None;
}

void USimulationSystemSubsystem::SetGlobalGraph(AGlobalGraph* GraphPtr)
{
	if (!IsValid(GraphPtr))
	{
		//if (ensureMsgf(
		//	IsValid(GlobalGraph),
		//	TEXT("Attempt to clear value of GlobalGraph in SimulationSystemSubsystem while value already null!")))
		//{
			GlobalGraph = nullptr;
		//}
	} else
	{
		ensureMsgf(
			!IsValid(GlobalGraph),
			TEXT("Attempt to set GlobalGraph [%s] in SimulationSystemSubsystem while other GlobalGraph [%s] is already set!"),
			*(GraphPtr->GetName()),
			*(GlobalGraph->GetName()));
		GlobalGraph = GraphPtr;
	}
}

AGlobalGraph* USimulationSystemSubsystem::GetGlobalGraph() const
{
	ensureMsgf(IsValid(GlobalGraph), TEXT("Attempt to access to invalid GlobalGraph!"));
	return GlobalGraph;
}
