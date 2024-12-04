// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimProfileBase.h"
#include "AISimProfileBase.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UAISimProfileBase : public USimProfileBase
{
public:
	virtual void OnRegistered_Implementation() override;
	virtual void OnCreated_Implementation() override;

	FORCEINLINE const TMap<FName, int>& GetCharacteristics(){return Characteristics;};
	FORCEINLINE const TMap<FName, float>& GetPoints(){return Points;};

private:
	GENERATED_BODY()

protected:
	
	UPROPERTY()
	TMap<FName, int> Characteristics;
	
	UPROPERTY()
	TMap<FName, float> Points;
    
};
