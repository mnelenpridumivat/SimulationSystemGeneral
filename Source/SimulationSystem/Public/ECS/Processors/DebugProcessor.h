// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "DebugProcessor.generated.h"

struct SIMULATIONSYSTEM_API FEntitiesForDebugContainer
{
	static FCriticalSection EntitiesCS;
	static TArray<FMassEntityHandle> Entities;
};

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UDebugProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
	UDebugProcessor();


protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
