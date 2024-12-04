#pragma once
#include "SimVertexID.h"

//#include "CoreMinimal.h"
//#include "SimVertexID.h"
//#include "Vertex.generated.h"

namespace Simulation
{
	class Edge;
	class SIMULATIONSYSTEM_API Vertex
	{
	public:
		Vertex(const FSimVertexID& VertexID, const FVector& Location);

		void AddEdge(const TWeakPtr<Edge>& NewEdge);
		const FVector& GetLocation() const {return Location;}
		
		const TArray<TWeakPtr<Edge>>& GetEdges() const {return Edges;}
		const FSimVertexID& GetVertexID() const {return VertexID;};

	protected:
		FSimVertexID VertexID = {static_cast<uint16>(-1), static_cast<uint8>(-1), static_cast<uint16>(-1)};
		FVector Location;

		TArray<TWeakPtr<Edge>> Edges;
		
	};
}
