// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActionPrecondition.generated.h"

struct FActionStorage;
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

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Key;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool Value;

public:

	void SetParentAction(UAction* Action);

	FORCEINLINE FName GetKey() const { return Key; }
	FORCEINLINE bool GetValue() const { return Value; }
	
};
