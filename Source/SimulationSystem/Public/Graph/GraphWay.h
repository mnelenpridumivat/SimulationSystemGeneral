#pragma once

#include "CoreMinimal.h"
#include "Edge.h"

#include "Vertex.h"

#include "GraphWay.generated.h"

constexpr size_t MAX_PATH_LENGTH = 16;

struct FGraphWayIDArray;

USTRUCT(BlueprintType)
struct FGraphWayVerticesArray
{
	GENERATED_BODY()

	FGraphWayVerticesArray() = default;
	FGraphWayVerticesArray(UObject* Context, const FGraphWayIDArray& Way);

	TWeakPtr<Simulation::Vertex> Vertices[MAX_PATH_LENGTH];
	size_t Num;
};

USTRUCT(BlueprintType)
struct FGraphWayIDArray
{
	GENERATED_BODY()

	FGraphWayIDArray() = default;
	FGraphWayIDArray(const FGraphWayVerticesArray& Way);

	FSimVertexID Vertices[MAX_PATH_LENGTH];
	size_t Num;
};

USTRUCT(BlueprintType)
struct FGraphWay
{
	friend struct FGraphTargetPositionFragment;
	GENERATED_BODY()

private:

	FGraphWayVerticesArray Way = {};
	size_t CurrentVertexIndex = 0;
	
public:

	FORCEINLINE FGraphWayVerticesArray& GetWay() {return Way;}

	const TWeakPtr<Simulation::Vertex>& GetStart() const {return Way.Vertices[0];}
	const TWeakPtr<Simulation::Vertex>& GetEnd() const {return Way.Vertices[Way.Num-1];}
	const TWeakPtr<Simulation::Vertex>& GetCurrent() const {return Way.Vertices[CurrentVertexIndex];}
	const TWeakPtr<Simulation::Vertex>& GetNext() const {return Way.Vertices[CurrentVertexIndex+1];}

	/*TArray<TWeakPtr<Simulation::Vertex>>::RangedForConstIteratorType begin() const
	{
		return Way.Vertices[0];
	}

	TArray<TWeakPtr<Simulation::Vertex>>::RangedForConstIteratorType end() const
	{
		return Way.end();
	}*/

	FORCEINLINE void StepForward(){++CurrentVertexIndex;}
	FORCEINLINE void StepBackward(){--CurrentVertexIndex;}
	
	bool IsValid() const
	{
		return Way.Num != 0;
	}

	void Serialize(FArchive& Ar, UObject* Context);
	
};