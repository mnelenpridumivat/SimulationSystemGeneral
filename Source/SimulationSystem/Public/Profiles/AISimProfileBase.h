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
	virtual void NativeOnRegistered() override;
	virtual void NativeOnCreated() override;

	FORCEINLINE const TMap<FName, int>& GetCharacteristics(){return CharacteristicsList;};
	FORCEINLINE const TMap<FName, float>& GetPoints(){return PointsList;};

private:
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category="Characteristics")
	TMap<FName, int> CharacteristicsList;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category="Characteristics")
	TMap<FName, float> PointsList;
    
};
