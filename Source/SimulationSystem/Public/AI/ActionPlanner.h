// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActionPlanner.generated.h"

class UActionPlan;
class UAction;
class UEvaluator;

USTRUCT(BlueprintType)
struct FActionStorage
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<FName, bool> State;

public:
	void Reset(){State.Reset();}
	void SetState(FName Key, bool Value); // Set initial value on key - for evaluators
	void UpdateState(FName Key, bool Value); // Update already existing value
	bool GetState(FName Key) const;
	
};

USTRUCT(BlueprintType)
struct FGOAPActionClass
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAction> ActionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;
};

FORCEINLINE uint32 GetTypeHash(const FGOAPActionClass& Struct)
{
	return HashCombine(GetTypeHash(Struct.ActionClass), GetTypeHash(Struct.Weight));
}

USTRUCT()
struct FGOAPAction
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UAction> Action;

	UPROPERTY()
	float Weight;
};

FORCEINLINE uint32 GetTypeHash(const FGOAPAction& Struct)
{
	return HashCombine(GetTypeHash(Struct.Action), GetTypeHash(Struct.Weight));
}

USTRUCT(BlueprintType)
struct FActionPlannerGoal
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, bool> State;
};

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
	TArray<FGOAPAction> ActionsObjs;

	UPROPERTY()
	FActionStorage ActionStorage;

	UPROPERTY()
	TObjectPtr<UActionPlan> Plan = nullptr;

	UPROPERTY()
	bool IsThinking = false; // if true, avoid any ActionStorage changes until made a decision

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSet<TSubclassOf<UEvaluator>> Evaluators;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSet<FGOAPActionClass> Actions;

public:

	void SetParentObject(UObject* Object);
	UObject* GetParentObject();
	void BeginPlay();

	/*
	 * Functions returns null - no way to reach goal
	 * Functions return UActionPlan with no steps - we already reached goal
	 * Functions returns UActionPlan with some steps - ok, need to do smth
	 */
	UFUNCTION(BlueprintCallable)
	UActionPlan* MakeDecision(const FActionPlannerGoal& Goal);
	
	FORCEINLINE FActionStorage& GetActionStorage(){return ActionStorage;}
};
