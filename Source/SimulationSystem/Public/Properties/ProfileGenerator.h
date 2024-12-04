// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
//#include "Engine/DataAsset.h"
#include "ProfileGenerator.generated.h"

class USimProfileBase;

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories)
class SIMULATIONSYSTEM_API UProfileGenerator : public UObject
{
public:
	//virtual bool CanEditChange(const FProperty* InProperty) const override;

	virtual USimProfileBase* GenerateProfile();

	TSubclassOf<USimProfileBase> GetProfileClass(){return ProfileClass;}

private:
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USimProfileBase> ProfileClass;
	
};
