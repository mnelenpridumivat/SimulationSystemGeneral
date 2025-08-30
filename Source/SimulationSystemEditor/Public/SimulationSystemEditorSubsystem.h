// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "SimulationSystemSubsystemStructs.h"
#include "SimulationSystemEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEMEDITOR_API USimulationSystemEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	USimulationSystemFunctionsImplementation* SimulationSystemFunctions = nullptr;

	UPROPERTY()
	TMap<FName, FPawnsOfClass> ClassComposedData = {};

public:
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void GetAllPawnClasses(TArray<FName>& OutPawnClasses);
};
