// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationTrait.h"
#include "SimulationTableTrait.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class SIMULATIONSYSTEM_API USimulationTableTrait : public USimulationTrait
{
	GENERATED_BODY()

#if WITH_EDITOR
	UFUNCTION()
	TArray<FName> GetRowNameOptions();
#endif

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanBeRandom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* DefaultsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "bCanBeRandom"))
	UDataTable* RandomsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(GetOptions="GetRowNameOptions"))
	FName DefaultRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "bCanBeRandom"))
	bool DefaultUseRandom;

public:
	UFUNCTION(BlueprintPure)
	bool CanBeRandom() const {return bCanBeRandom;}
	
	UFUNCTION(BlueprintNativeEvent)
	void SetupDefaultTableSettings(FSimulationTraitOverrides& data);

	// This is a post-spawn init, possibly with spawning other entities
	virtual void SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity, const FSimulationTraitOverrides& OverrideData);

	UFUNCTION(BlueprintPure)
	UDataTable* GetDefaultsDataTable() const {return DefaultsDataTable;}

	UFUNCTION(BlueprintPure)
	UDataTable* GetRandomsDataTable() const {return RandomsDataTable;}
};
