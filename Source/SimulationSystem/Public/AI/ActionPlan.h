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

public:

	void SetActions(const TArray<UAction*>& Actions);
	
};
