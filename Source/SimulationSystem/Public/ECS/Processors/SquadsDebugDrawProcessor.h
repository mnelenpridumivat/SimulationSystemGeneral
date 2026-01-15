// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "SquadsDebugDrawProcessor.generated.h"

USTRUCT()
struct SIMULATIONSYSTEM_API FSquadsDebugDrawContainer
{
	GENERATED_BODY()

	static FCriticalSection Lock;
	static TArray<FVector> GraphPositions;
	static TArray<FVector> EntityPositions;
	static TArray<TPair<FVector, FVector>> WayLines;
};

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USquadsDebugDrawProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
	USquadsDebugDrawProcessor();


protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
