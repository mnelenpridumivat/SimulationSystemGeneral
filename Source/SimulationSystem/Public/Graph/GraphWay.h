#pragma once

#include "CoreMinimal.h"
#include "Edge.h"

#include "Vertex.h"

#include "GraphWay.generated.h"

USTRUCT(BlueprintType)
struct FGraphWay
{
	GENERATED_BODY()

private:

	TArray<TWeakPtr<Simulation::Vertex>> Way = {};
	size_t CurrentVertexIndex = 0;
	
public:

	FORCEINLINE TArray<TWeakPtr<Simulation::Vertex>>& GetWay() {return Way;}

	const TWeakPtr<Simulation::Vertex>& GetStart() const {return Way[0];}
	const TWeakPtr<Simulation::Vertex>& GetEnd() const {return Way.Last();}
	const TWeakPtr<Simulation::Vertex>& GetCurrent() const {return Way[CurrentVertexIndex];}
	const TWeakPtr<Simulation::Vertex>& GetNext() const {return Way[CurrentVertexIndex+1];}

	TArray<TWeakPtr<Simulation::Vertex>>::RangedForConstIteratorType begin() const
	{
		return Way.begin();
	}

	TArray<TWeakPtr<Simulation::Vertex>>::RangedForConstIteratorType end() const
	{
		return Way.end();
	}

	FORCEINLINE void StepForward(){++CurrentVertexIndex;}
	FORCEINLINE void StepBackward(){--CurrentVertexIndex;}
	
	bool IsValid() const
	{
		return Way.Num() != 0;
	}

	void Serialize(FArchive& Ar, UObject* Context);
	
};