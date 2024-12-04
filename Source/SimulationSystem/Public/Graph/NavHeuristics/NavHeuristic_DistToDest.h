// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavHeuristic_Base.h"
#include "NavHeuristic_DistToDest.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UNavHeuristic_DistToDest : public UNavHeuristic_Base
{
public:
	virtual float NativeEvaluate(
		float CurrentScore, 
		const TWeakPtr<Simulation::Vertex>& Start,
		const TWeakPtr<Simulation::Vertex>& Current,
		const TWeakPtr<Simulation::Vertex>& Next,
		const TWeakPtr<Simulation::Vertex>& Dest
		) override;

private:
	GENERATED_BODY()
};
