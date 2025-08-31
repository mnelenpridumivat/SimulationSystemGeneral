// Fill out your copyright notice in the Description page of Project Settings.


#include "ProfileComponentCamp.h"

#include "AISimProfileSquad.h"
#include "GlobalGraph.h"
#include "SimProfileBase.h"
#include "SimProfileCamp.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemSettings.h"
#include "SimulationSystemSubsystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UProfileComponentCamp::UProfileComponentCamp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProfileComponentCamp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

USimProfileBase* UProfileComponentCamp::CreateNewProfile_Implementation()
{
	auto NewProfile = Super::CreateNewProfile_Implementation();
	ensureMsgf(NewProfile->IsA(
		USimProfileCamp::StaticClass())
		||NewProfile->GetClass()->ImplementsInterface(USimProfileContainer::StaticClass()),
		TEXT("Invalid profile class [%s] for profile component [%s]"), *Profile->GetClass()->GetName(), *GetClass()->GetName());
	const auto FunctionSubsystem = USimulationFunctionLibrary::GetSimulationSystemSubsystem(GetWorld());
	const auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	if (ensure(GlobalGraph))
	{
		for(auto& elem : StartSquads)
		{
			FGeneratorHandleSquad handle;
			handle.GeneratorClass = UProfileGeneratorSquad::StaticClass();
			handle.SquadName = elem;
			auto NewSquad = FunctionSubsystem->ExecuteGenerator(GetWorld(), handle);
			GlobalGraph->RegisterChildProfile(NewSquad, NewProfile);
			ISimProfileContainer::Execute_AddItem(NewProfile, NewSquad);
		}
	}
	return NewProfile;
}


// Called every frame
void UProfileComponentCamp::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

