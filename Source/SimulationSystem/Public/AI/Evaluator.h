// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Evaluator.generated.h"

class UActionPlanner;
/**
 * 
 */
UCLASS(Blueprintable)
class SIMULATIONSYSTEM_API UEvaluator : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UActionPlanner> ParentPlanner;

public:
	void SetParentPlanner(UActionPlanner* Planner);

	UFUNCTION(BlueprintNativeEvent)
	void Evaluate();

protected:
	/*
	 * This function designed to called once per key
	 */
	UFUNCTION(BlueprintCallable)
	void SetState(FName key, bool value) const;

	UFUNCTION(BlueprintPure)
	UActionPlannerOwner* GetOwningObject() const;
	
};
