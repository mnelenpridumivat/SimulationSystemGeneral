#include "Graph/Vertex.h"

const FSimVertexID FSimVertexID::Invalid = {static_cast<uint16>(-1), static_cast<uint8>(-1), static_cast<uint16>(-1)};

Simulation::Vertex::Vertex(const FSimVertexID& VertexID, const FVector& Location)
{
	this->VertexID = VertexID;
	this->Location = Location;
}

void Simulation::Vertex::AddEdge(const TWeakPtr<Edge>& NewEdge)
{
	Edges.Add(NewEdge);
}
