// Fill out your copyright notice in the Description page of Project Settings.


#include "NavHeuristics/NavHeuristic_Base.h"

#include "Vertex.h"

float UNavHeuristic_Base::NativeEvaluate(
	float CurrentScore,
	const TWeakPtr<Simulation::Vertex>& Start,
	const TWeakPtr<Simulation::Vertex>& Current,
	const TWeakPtr<Simulation::Vertex>& Next,
	const TWeakPtr<Simulation::Vertex>& Dest
	)
{
	return Evaluate_Implementation(CurrentScore, Start.Pin()->GetVertexID(), Current.Pin()->GetVertexID(), Next.Pin()->GetVertexID(), Dest.Pin()->GetVertexID());
}

float UNavHeuristic_Base::Evaluate_Implementation(
	float CurrentScore,
	const FSimVertexID& Start,
	const FSimVertexID& Current,
	const FSimVertexID& Next,
	const FSimVertexID& Dest
	)
{
	return CurrentScore;
}
