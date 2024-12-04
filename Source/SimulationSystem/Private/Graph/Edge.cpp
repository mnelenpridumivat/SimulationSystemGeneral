#include "Graph/Edge.h"

#include "Vertex.h"

Simulation::Edge::Edge(const TWeakPtr<Vertex>& VertexOne, const TWeakPtr<Vertex>& VertexTwo)
{
	this->VertexOne = VertexOne;
	this->VertexTwo = VertexTwo;
}

TWeakPtr<Simulation::Vertex> Simulation::Edge::GetOpposite(TWeakPtr<Simulation::Vertex> Vertex)
{
	if(Vertex == VertexOne)
	{
		return VertexTwo;
	}
	if(Vertex == VertexTwo)
	{
		return VertexOne;
	}
	return nullptr;
}
