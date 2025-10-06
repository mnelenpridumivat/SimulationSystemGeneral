// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActionPrecondition.generated.h"

class UAction;
/**
 * 
 */
UCLASS(Blueprintable)
class SIMULATIONSYSTEM_API UActionPrecondition : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UAction> ParentAction;

public:

	void SetParentAction(UAction* Action);
};
