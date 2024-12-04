// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimVertexID.h"
#include "Containers/Union.h"
#include "Interfaces/SimProfileContainer.h"
#include "UObject/Object.h"
#include "SimProfileHolder.generated.h"

class USimProfileBase;

namespace Simulation
{
	class Vertex;
}

USTRUCT(BlueprintType)
struct FSimProfileHolder
{
	GENERATED_BODY()
	
	using TSimProfileHolder = TUnion<TWeakPtr<Simulation::Vertex>, USimProfileBase*>;

	TSimProfileHolder Holder;

	static FSimProfileHolder FromVertex(UWorld* World, const FSimVertexID& VertexID);
	static FSimProfileHolder FromProfile(USimProfileBase* Profile);
	
};