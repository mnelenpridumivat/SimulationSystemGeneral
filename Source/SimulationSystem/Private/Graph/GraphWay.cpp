#include "GraphWay.h"

#include "GlobalGraph.h"
#include "SimulationFunctionLibrary.h"

void FGraphWay::Serialize(FArchive& Ar, UObject* Context)
{
	TArray<FSimVertexID> WayVerticesIDs;
	if(!Ar.IsLoading())
	{
		for(auto& elem : Way)
		{
			WayVerticesIDs.Add(elem.Pin()->GetVertexID());
		}
	}
	Ar << WayVerticesIDs;
	Ar << CurrentVertexIndex;
	if(Ar.IsLoading())
	{
		Way.Empty();
		auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(Context);
		for(auto& elem : WayVerticesIDs)
		{
			Way.Add(GlobalGraph->GetVertexByID(elem));
		}
	}
}
