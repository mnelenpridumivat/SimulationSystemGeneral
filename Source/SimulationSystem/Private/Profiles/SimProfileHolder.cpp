#include "Profiles/SimProfileHolder.h"

#include "GlobalGraph.h"
#include "SimulationFunctionLibrary.h"
#include "Profiles/SimProfileBase.h"

FSimProfileHolder FSimProfileHolder::FromVertex(UWorld* World, const FSimVertexID& VertexID)
{
	FSimProfileHolder Holder;
	Holder.Holder.SetSubtype<TWeakPtr<Simulation::Vertex>>(USimulationFunctionLibrary::GetGlobalGraph(World)->GetVertexByID(VertexID));
	return Holder;
}

FSimProfileHolder FSimProfileHolder::FromProfile(USimProfileBase* Profile)
{
	ensureMsgf(Profile->GetClass()->ImplementsInterface(USimProfileContainer::StaticClass()), TEXT("Profile [%s] has no SimProfileContainer interface!"), *Profile->GetName());
	FSimProfileHolder Holder;
	Holder.Holder.SetSubtype<USimProfileBase*>(Profile);
	return Holder;
}
