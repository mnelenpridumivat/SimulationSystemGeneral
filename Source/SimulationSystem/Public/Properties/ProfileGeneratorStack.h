// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProfileGenerator.h"
#include "ProfileGeneratorStack.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UProfileGeneratorStack : public UProfileGenerator
{
public:
	virtual USimProfileBase* GenerateProfileOld() override;

private:
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin="0", UIMin="0"))
	int AmountMin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin="0", UIMin="0"))
	int AmountMax;
};
