// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\Profiles\Tasks\SquadTask_MoveToCamp.h"

#include "AISimProfileSquad.h"
#include "GlobalGraph.h"
#include "NavHeuristic_DistToDest.h"
#include "SimulationFunctionLibrary.h"
#include "Vertex.h"

void USquadTask_MoveToCamp::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	Way.Serialize(Ar, GetWorld());
}

void USquadTask_MoveToCamp::BeginTask_Offline_Implementation(UAISimProfileSquad* Squad)
{
	Super::BeginTask_Offline_Implementation(Squad);
	if(!Way.IsValid() || CheckTaskFinished_Offline_Implementation(Squad))
	{
		return;
	}
	LocalDistance = FVector::Dist(Way.GetStart().Pin()->GetLocation(), Way.GetNext().Pin()->GetLocation());
	USimulationFunctionLibrary::GetGlobalGraph(GetWorld())->SetProfileLocation_World(Squad, Way.GetStart().Pin()->GetVertexID());
	Squad->SetOnlineLocation(Way.GetStart().Pin()->GetLocation());
}

void USquadTask_MoveToCamp::ExecuteTask_Offline_Implementation(UAISimProfileSquad* Squad, float DeltaTime)
{
	Super::ExecuteTask_Offline_Implementation(Squad, DeltaTime);
	LocalDistanceCompleted += Squad->GetOfflineSpeed()*DeltaTime;
	while (LocalDistanceCompleted >= LocalDistance)
	{
		LocalDistanceCompleted -= LocalDistance;
		Way.StepForward();
		USimulationFunctionLibrary::GetGlobalGraph(GetWorld())->SetProfileLocation_World(Squad, Way.GetCurrent().Pin()->GetVertexID());
		if(CheckTaskFinished_Offline_Implementation(Squad))
		{
			break;
		}
		LocalDistance = FVector::Dist(Way.GetCurrent().Pin()->GetLocation(), Way.GetNext().Pin()->GetLocation());
		Squad->SetOnlineLocation(Way.GetCurrent().Pin()->GetLocation());
	}
}

void USquadTask_MoveToCamp::FinishTask_Offline_Implementation(UAISimProfileSquad* Squad, bool Success)
{
	Super::FinishTask_Offline_Implementation(Squad, Success);
}

bool USquadTask_MoveToCamp::CheckTaskFinished_Offline_Implementation(UAISimProfileSquad* Squad)
{
	return Way.GetCurrent() == Way.GetEnd();
}

void USquadTask_MoveToCamp::BeginTask_Buffer_Implementation(UAISimProfileSquad* Squad)
{
	Super::BeginTask_Buffer_Implementation(Squad);
	BeginTask_Offline(Squad);
}

void USquadTask_MoveToCamp::ExecuteTask_Buffer_Implementation(UAISimProfileSquad* Squad, float DeltaTime)
{
	Super::ExecuteTask_Buffer_Implementation(Squad, DeltaTime);
	ExecuteTask_Offline(Squad, DeltaTime);
}

void USquadTask_MoveToCamp::FinishTask_Buffer_Implementation(UAISimProfileSquad* Squad, bool Success)
{
	Super::FinishTask_Buffer_Implementation(Squad, Success);
	FinishTask_Offline(Squad, Success);
}

bool USquadTask_MoveToCamp::CheckTaskFinished_Buffer_Implementation(UAISimProfileSquad* Squad)
{
	return CheckTaskFinished_Offline(Squad);
}

void USquadTask_MoveToCamp::SetupTask(const FSimVertexID& Start, const FSimVertexID& Dest)
{
	Way = USimulationFunctionLibrary::FindWay(GetWorld(), Start, Dest, {NewObject<UNavHeuristic_DistToDest>()});
}
