// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskDebug/TaskDebugMoveToCamp.h"

#include "DrawDebugHelpers.h"
#include "SquadTask_MoveToCamp.h"
#include "Vertex.h"

void UTaskDebugMoveToCamp::VisualizeTaskInfo(float DeltaTime, UAISimProfileSquad* Squad, USquadTaskBase* Task)
{
	Super::VisualizeTaskInfo(DeltaTime, Squad, Task);
	auto CastedTask = Cast<USquadTask_MoveToCamp>(Task);
	auto& Way = CastedTask->GetWay();
	TWeakPtr<Simulation::Vertex> Prev = nullptr;
	for(auto& elem : Way)
	{
		if(elem == Way.GetCurrent() && Way.GetCurrent() != Way.GetEnd())
		{
			Prev = elem;
			DrawDebugCapsule(
				GetWorld(),
				FMath::Lerp(
					elem.Pin()->GetLocation(),
					Way.GetNext().Pin()->GetLocation(),
					CastedTask->GetLocalDistanceCompleted()/CastedTask->GetLocalDistance()
					),
				200.0f,
				100.0f,
				FRotator::ZeroRotator.Quaternion(),
				FColor::Red,
				false,
				DeltaTime*1.1f,
				0,
				10
				);
			continue;
		}
		if(Prev.IsValid())
		{
			DrawDebugLine(
				GetWorld(),
				Prev.Pin()->GetLocation(),
				elem.Pin()->GetLocation(),
				FColor::Red,
				false,
				DeltaTime*1.1f,
				0,
				15
				);
			Prev = elem;
		}
	}
}

void UTaskDebugMoveToCamp::ImGuiTaskInfo(float DeltaTime, UAISimProfileSquad* Squad, USquadTaskBase* Task)
{
	Super::ImGuiTaskInfo(DeltaTime, Squad, Task);
	
}
