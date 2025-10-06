// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommunityRelationTable.generated.h"

USTRUCT(BlueprintType)
struct FCommunityRelations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize)
	TMap<FName, int> Relations;
	
};

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UCommunityRelationTable : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<FName> Communities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, EditFixedSize)
	TMap<FName, FCommunityRelations> Relations;

public:

	UFUNCTION(CallInEditor)
	void Validate();

	/*
	 * Check how A relates to B. If table is valid, CheckRelations(A, B) == CheckRelations(B, A)
	 */
	UFUNCTION(BlueprintPure)
	int CheckRelations(FName A, FName B);
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
