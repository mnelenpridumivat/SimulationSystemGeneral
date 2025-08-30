// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
//#include "Engine/DataAsset.h"
#include "ProfileGenerator.generated.h"

class UProfileGenerator;
class USimProfileBase;

USTRUCT(BlueprintType)
struct SIMULATIONSYSTEM_API FGeneratorHandleBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UProfileGenerator> GeneratorClass;
	
	virtual ~FGeneratorHandleBase() = default;
	virtual void SetupGenerator(UProfileGenerator* Generator) const {}
};

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories)
class SIMULATIONSYSTEM_API UProfileGenerator : public UObject
{
public:
	//virtual bool CanEditChange(const FProperty* InProperty) const override;

	virtual USimProfileBase* GenerateProfile();

private:
	GENERATED_BODY()
	
};
