// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActionPlan.generated.h"

class UAction;
/**
 * Class containing actions in order to reach our goal
 */
UCLASS()
class SIMULATIONSYSTEM_API UActionPlan : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UAction*> PlanActions;

	UPROPERTY()
	int CurrentStep = 0;

	UPROPERTY()
	bool HasStepChanged = true;

	UPROPERTY()
	bool bCanExecute = true;

public:

	void SetActions(const TArray<UAction*>& Actions);

	UFUNCTION(BlueprintPure)
	bool CanExecute() const {return HasAnySteps() && bCanExecute;}

	UFUNCTION(BlueprintPure)
	bool HasAnySteps() const {return !PlanActions.IsEmpty();}

	UFUNCTION(BlueprintCallable)
	void Execute();
	
	
};
