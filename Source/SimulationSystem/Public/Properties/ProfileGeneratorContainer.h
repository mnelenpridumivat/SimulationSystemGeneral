// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProfileGenerator.h"
#include "ProfileGeneratorItem.h"
#include "ProfileGeneratorContainer.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UProfileGeneratorContainer : public UProfileGenerator
{
public:
	virtual USimProfileBase* GenerateProfileOld() override;

private:
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FProfileGeneratorItem> Items;
};
