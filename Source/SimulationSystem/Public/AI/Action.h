// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Action.generated.h"

struct FActionPlannerGoal;
struct FActionStorage;
class UActionPlanner;
class UActionEffect;
class UActionPrecondition;
/**
 * 
 */
UCLASS(Blueprintable)
class SIMULATIONSYSTEM_API UAction : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UActionPlanner> ParentPlanner;

	UPROPERTY()
	TArray<TObjectPtr<UActionPrecondition>> PreconditionsObjs;

	UPROPERTY()
	TArray<TObjectPtr<UActionEffect>> EffectsObjs;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<TSubclassOf<UActionPrecondition>> Preconditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<TSubclassOf<UActionEffect>> Effects;

public:

	void SetParentPlanner(UActionPlanner* Planner);
	void BeginPlay();

	bool CheckPreconditions(const FActionStorage& Storage);
	bool CheckSelfPreconditions();

	bool CheckIfWeCanCreateState(const FActionPlannerGoal& Storage);
	void GetRequiredStartGoal(FActionPlannerGoal& Storage);
	
};
