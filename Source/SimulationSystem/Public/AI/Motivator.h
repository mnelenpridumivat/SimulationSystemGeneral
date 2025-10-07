// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Motivator.generated.h"

struct FActionPlannerGoal;
class UActionPlanner;
/**
 * 
 */
UCLASS(Blueprintable)
class SIMULATIONSYSTEM_API UMotivator : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UActionPlanner> ParentPlanner;
	
public:

	void SetParentPlanner(UActionPlanner* Planner);

	bool NativeFindGoal(FActionPlannerGoal& Goal);
	
protected:

	UFUNCTION(BlueprintNativeEvent)
	bool FindGoal(FActionPlannerGoal& Goal);

	UFUNCTION(BlueprintPure)
	UObject* GetOwningObject() const;

	UFUNCTION(BlueprintPure)
	bool GetPlannerKey(FName Key);
};
