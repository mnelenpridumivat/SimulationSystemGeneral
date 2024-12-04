// Fill out your copyright notice in the Description page of Project Settings.


#include "NavHeuristics/NavHeuristic_DistToDest.h"

#include "Vertex.h"

float UNavHeuristic_DistToDest::NativeEvaluate(
		float CurrentScore, 
		const TWeakPtr<Simulation::Vertex>& Start,
		const TWeakPtr<Simulation::Vertex>& Current,
		const TWeakPtr<Simulation::Vertex>& Next,
		const TWeakPtr<Simulation::Vertex>& Dest
		)
{
	return CurrentScore+FVector::Dist(Current.Pin()->GetLocation(), Dest.Pin()->GetLocation());
}
