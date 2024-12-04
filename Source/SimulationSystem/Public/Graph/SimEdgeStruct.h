#pragma once

#include "CoreMinimal.h"
#include "SimVertexID.h"
#include "SimEdgeStruct.generated.h"

USTRUCT()
struct FSimEdgeStruct
{
	GENERATED_BODY()

	UPROPERTY()
	FSimVertexID VertexOne;

	UPROPERTY()
	FSimVertexID VertexTwo;

	bool operator==(const FSimEdgeStruct& other);
	bool operator==(FSimEdgeStruct&& other);
	bool operator!=(const FSimEdgeStruct& other);
	bool operator!=(FSimEdgeStruct&& other);

	FORCEINLINE bool IsValid()
	{
		return VertexOne.IsValid()&&VertexTwo.IsValid();
	}
	
};

/*namespace Simulation
{
	static const FSimEdgeStruct& GetInvalidEdge();
}*/

inline bool FSimEdgeStruct::operator==(const FSimEdgeStruct& other)
{
	return VertexOne == other.VertexOne && VertexTwo == other.VertexTwo
	|| VertexOne == other.VertexTwo && VertexTwo == other.VertexOne;
}

inline bool FSimEdgeStruct::operator==(FSimEdgeStruct&& other)
{
	return VertexOne == other.VertexOne && VertexTwo == other.VertexTwo
	|| VertexOne == other.VertexTwo && VertexTwo == other.VertexOne;
}

inline bool FSimEdgeStruct::operator!=(const FSimEdgeStruct& other)
{
	return !(*this==other);
}

inline bool FSimEdgeStruct::operator!=(FSimEdgeStruct&& other)
{
	return *this!=other;
}

/*inline const FSimEdgeStruct& Simulation::GetInvalidEdge()
{
	static const FSimEdgeStruct InvalidEdge = {Simulation::GetInvalidVertex(), Simulation::GetInvalidVertex()};
	return InvalidEdge;
}*/
