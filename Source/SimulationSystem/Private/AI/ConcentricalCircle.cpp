// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcentricalCircle.h"

#include "AISimProfileSquad.h"
#include "GlobalGraph.h"
#include "GraphAsset.h"
#include "SimulationFunctionLibrary.h"

void UConcentricalCircle::SetCenterObject(TObjectPtr<USimProfileBase> SimProfile)
{
	if(!ensureMsgf(
			IsValid(SimProfile),
			TEXT("Attempt to set invalid CenterObject in ConcentricalCircle [%s]"),
			*GetName())
		|| !ensureMsgf(
			!IsValid(CenterObject),
			TEXT("Attempt to set CenterObject [%s] in ConcentricalCircle [%s] while other CenterObject [%s] is already set!"),
			*SimProfile->GetName(),
			*GetName(),
			*CenterObject->GetName()))
	{
		return;
	}
	CenterObject = SimProfile;
}

void UConcentricalCircle::RegisterSquad(UAISimProfileSquad* NewSquad)
{
	if(!ensureMsgf(
		IsValid(NewSquad),
		TEXT("Attempt to register invalid squad in ConcentricalCircle [%s]"),
		*GetName()))
	{
		return;
	}
	if(IsSuitableFor(NewSquad, bOverrideVeryCloseDetection, VeryCloseRadius, VeryCloseDistanceVerification))
	{
		ensureMsgf(
			!VeryCloseSquads.Contains(NewSquad),
			TEXT("Attempt to register squad [%s] in ConcentricalCircle [%s] twice!"),
			*NewSquad->GetName(),
			*GetName());
		VeryCloseSquads.Add(NewSquad);
		return;
	}
	if(IsSuitableFor(NewSquad, bOverrideCloseDetection, CloseRadius, CloseDistanceVerification))
	{
		ensureMsgf(
			!CloseSquads.Contains(NewSquad),
			TEXT("Attempt to register squad [%s] in ConcentricalCircle [%s] twice!"),
			*NewSquad->GetName(),
			*GetName());
		CloseSquads.Add(NewSquad);
		return;
	}
	if(IsSuitableFor(NewSquad, bOverrideFarDetection, FarRadius, FarDistanceVerification))
	{
		ensureMsgf(
			!FarSquads.Contains(NewSquad),
			TEXT("Attempt to register squad [%s] in ConcentricalCircle [%s] twice!"),
			*NewSquad->GetName(),
			*GetName());
		FarSquads.Add(NewSquad);
		return;
	}
	ensureMsgf(
		!VeryFarSquads.Contains(NewSquad),
		TEXT("Attempt to register squad [%s] in ConcentricalCircle [%s] twice!"),
		*NewSquad->GetName(),
		*GetName());
	VeryFarSquads.Add(NewSquad);
	return;
}

void UConcentricalCircle::UnregisterSquad(UAISimProfileSquad* RemovedSquad)
{
	VeryCloseSquads.Remove(RemovedSquad);
	CloseSquads.Remove(RemovedSquad);
	FarSquads.Remove(RemovedSquad);
	VeryFarSquads.Remove(RemovedSquad);
}

void UConcentricalCircle::UpdateSquad(UAISimProfileSquad* UpdatedSquad)
{
	UnregisterSquad(UpdatedSquad);
	RegisterSquad(UpdatedSquad);
}

void UConcentricalCircle::GetVeryCloseSquads(TArray<UAISimProfileSquad*>& Squads)
{
	Squads = VeryCloseSquads.Array();
}

void UConcentricalCircle::GetCloseSquads(TArray<UAISimProfileSquad*>& Squads)
{
	Squads = CloseSquads.Array();
}

void UConcentricalCircle::GetFarSquads(TArray<UAISimProfileSquad*>& Squads)
{
	Squads = FarSquads.Array();
}

void UConcentricalCircle::GetVeryFarSquads(TArray<UAISimProfileSquad*>& Squads)
{
	Squads = VeryFarSquads.Array();
}

bool UConcentricalCircle::IsSuitableFor(UAISimProfileSquad* Squad, bool UseOverride, float DefDistance,
                                        FDistanceVerificationOverride& Override) const
{
	if(!ensureMsgf(
			IsValid(CenterObject),
			TEXT("ConcentricalCircle [%s] has invalid CenterObject!"),
			*GetName())
		|| !ensureMsgf(
			IsValid(Squad),
			TEXT("Attempt to check invalid squad in ConcentricalCircle [%s]!"),
			*GetName()))
	{
		return false;
	}
	if(UseOverride)
	{
		if(!ensureMsgf(
			Override.IsBound(),
			TEXT("Unable to process check for squad [%s] in ConcentricalCircle [%s] (inside [%s]) because no override is bound!"),
			*Squad->GetName(),
			*GetName(),
			*CenterObject->GetName()))
		{
			return false;
		}
		return Override.Execute(CenterObject, Squad);
	}
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	if(!ensureMsgf(
		IsValid(GlobalGraph),
		TEXT("Unable to process check in ConcentricalCircle [%s] because cannot access GlobalGraph!"),
		*GetName()))
	{
		return false;
	}
	FVector CenterLocation, SquadLocation;
	{
		auto CenterLocationVertex = GlobalGraph->GetProfileLocationOnGraph(CenterObject);
		if(!ensureMsgf(
				CenterLocationVertex.IsValid(),
				TEXT("CenterObject (from object [%s]) in ConcentricalCircle [%s] has invalid location on GlobalGraph!"),
				*CenterObject->GetName(),
				*GetName())
			|| ensureMsgf(
				IsValid(GlobalGraph->GetChunkByID(CenterLocationVertex)),
				TEXT("CenterObject [%s] has invalid LocalGraph index!"),
				*CenterObject->GetName()))
		{
			return false;
		}
		CenterLocation = GlobalGraph->GetVertexLocationByID(CenterLocationVertex);
	}
	{
		auto SquadLocationVertex = GlobalGraph->GetProfileLocationOnGraph(CenterObject);
		if(!ensureMsgf(
				SquadLocationVertex.IsValid(),
				TEXT("Squad [%s] has invalid location on GlobalGraph!"),
				*Squad->GetName(),
				*GetName())
			|| ensureMsgf(
				IsValid(GlobalGraph->GetChunkByID(SquadLocationVertex)),
				TEXT("Squad [%s] has invalid LocalGraph index!"),
				*Squad->GetName()))
		{
			return false;
		}
		SquadLocation = GlobalGraph->GetVertexLocationByID(SquadLocationVertex);
	}
	return FVector::Distance(CenterLocation, SquadLocation) < DefDistance;
}
