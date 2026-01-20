#include "GraphWay.h"

#include "GlobalGraph.h"
#include "SimulationFunctionLibrary.h"

FGraphWayVerticesArray::FGraphWayVerticesArray(UObject* Context, const FGraphWayIDArray& Way)
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(Context);
	if (!ensure(GlobalGraph))
	{
		return;
	}
	this->Num = Way.Num;
	for (int i = 0; i < this->Num; i++)
	{
		Vertices[i] = GlobalGraph->GetVertexByID(Way.Vertices[i]);
	}
}

FGraphWayIDArray::FGraphWayIDArray(const FGraphWayVerticesArray& Way)
{
	this->Num = Way.Num;
	for (int i = 0; i < this->Num; i++)
	{
		Vertices[i] = Way.Vertices[i].Pin()->GetVertexID();
	}
}

void FGraphWay::Serialize(FArchive& Ar, UObject* Context)
{
	TArray<FSimVertexID> WayVerticesIDs;
	if(!Ar.IsLoading())
	{
		for (size_t i = 0; i < Way.Num; ++i)
		{
			WayVerticesIDs.Add(Way.Vertices[i].Pin()->GetVertexID());
		}
	}
	Ar << WayVerticesIDs;
	Ar << CurrentVertexIndex;
	if(Ar.IsLoading())
	{
		auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(Context);
		Way.Num = WayVerticesIDs.Num();
		for (size_t i = 0; i < WayVerticesIDs.Num(); ++i)
		{
			Way.Vertices[i] = GlobalGraph->GetVertexByID(WayVerticesIDs[i]);
		}
	}
}
