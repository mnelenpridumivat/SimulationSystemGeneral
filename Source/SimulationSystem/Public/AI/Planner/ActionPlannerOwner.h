// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "SimulationSystemProfileType.h"
#include "UObject/Object.h"
#include "ActionPlannerOwner.generated.h"

USTRUCT(BlueprintType)
struct FFragmentProperties
{
	GENERATED_BODY()

	UPROPERTY()
	FInstancedStruct Struct;

	TArray<FProperty*> Props;
	
};

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UActionPlannerOwner : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	UObject* ObjectOwner = nullptr;

	FMassEntityHandle EntityOwner;
	
public:

	void SetObjectOwner(UObject* NewObjectOwner);
	void SetEntityOwner(FMassEntityHandle NewEntityOwner);

	ESimualtionSystemProfileType GetProfileType() const;

	bool GetFragment(UScriptStruct* Struct, FFragmentProperties& Out);

	FMassEntityHandle GetEntityOwner() const;
	UObject* GetObjectOwner() const;
};
