#pragma once

namespace Simulation
{
	class Vertex;
	class SIMULATIONSYSTEM_API Edge
	{
	public:
		Edge(const TWeakPtr<Vertex>& VertexOne, const TWeakPtr<Vertex>& VertexTwo);

		TWeakPtr<Vertex> GetOpposite(TWeakPtr<Simulation::Vertex> Vertex);
		TWeakPtr<Vertex> GetVertexOne(){return VertexOne;}
		TWeakPtr<Vertex> GetVertexTwo(){return VertexTwo;}

	private:
		TWeakPtr<Vertex> VertexOne;
		TWeakPtr<Vertex> VertexTwo;
	
	};
}
