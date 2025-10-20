// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "GraphMoveToProcessor.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEMECS_API UGraphMoveToProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
	UGraphMoveToProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
