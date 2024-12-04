// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Edge.h"
#include "SimVertexID.h"
#include "UObject/Object.h"
#include "NavHeuristic_Base.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UNavHeuristic_Base : public UObject
{
	GENERATED_BODY()

public:

	virtual float NativeEvaluate(
		float CurrentScore, 
		const TWeakPtr<Simulation::Vertex>& Start,
		const TWeakPtr<Simulation::Vertex>& Current,
		const TWeakPtr<Simulation::Vertex>& Next,
		const TWeakPtr<Simulation::Vertex>& Dest
		);

	UFUNCTION(BlueprintNativeEvent)
	float Evaluate(
		float CurrentScore,
		const FSimVertexID& Start,
		const FSimVertexID& Current,
		const FSimVertexID& Next,
		const FSimVertexID& Dest
		);
	
};
