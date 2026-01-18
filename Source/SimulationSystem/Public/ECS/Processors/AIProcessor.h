// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "AIProcessor.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UAIProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
	UAIProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
