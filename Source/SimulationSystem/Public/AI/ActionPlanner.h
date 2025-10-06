// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActionPlanner.generated.h"

class UAction;
class UEvaluator;
/**
 * 
 */
UCLASS(Blueprintable)
class SIMULATIONSYSTEM_API UActionPlanner : public UObject
{
	GENERATED_BODY()

	// Object which uses this evaluator (Profile, Bar, Actor...)
	UPROPERTY()
	TObjectPtr<UObject> ParentObject;

	UPROPERTY()
	TArray<TObjectPtr<UEvaluator>> EvaluatorsObjs;

	UPROPERTY()
	TArray<TObjectPtr<UAction>> ActionsObjs;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<TSubclassOf<UEvaluator>> Evaluators;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<TSubclassOf<UAction>> Actions;

public:

	void SetParentObject(UObject* Object);

	void BeginPlay();
	
};
