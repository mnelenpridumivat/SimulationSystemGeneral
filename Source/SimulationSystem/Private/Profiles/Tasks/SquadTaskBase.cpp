// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\Profiles\Tasks\SquadTaskBase.h"

#include "AISimProfileSquad.h"
#include "GraphSerialized.h"
#include "SimulationFunctionLibrary.h"

void USquadTaskBase::BeginTask(UAISimProfileSquad* Squad)
{
	BeginTask_Always(Squad);
	switch (Squad->GetSimLevel())
	{
	case ESimulationLevels::Online:
		{
			BeginTask_Online(Squad);
			break;
		}
	case ESimulationLevels::Buffered:
		{
			BeginTask_Buffer(Squad);
			break;
		}
	case ESimulationLevels::Offline:
		{
			BeginTask_Offline(Squad);
			break;
		}
	}
}

void USquadTaskBase::ExecuteTask(UAISimProfileSquad* Squad, float DeltaTime)
{
	ExecuteTask_Always(Squad, DeltaTime);
	switch (Squad->GetSimLevel())
	{
	case ESimulationLevels::Online:
		{
			ExecuteTask_Online(Squad, DeltaTime);
			break;
		}
	case ESimulationLevels::Buffered:
		{
			ExecuteTask_Buffer(Squad, DeltaTime);
			break;
		}
	case ESimulationLevels::Offline:
		{
			ExecuteTask_Offline(Squad, DeltaTime);
			break;
		}
	}
}

void USquadTaskBase::FinishTask(UAISimProfileSquad* Squad, bool Success)
{
	FinishTask_Always(Squad, Success);
	switch (Squad->GetSimLevel())
	{
	case ESimulationLevels::Online:
		{
			FinishTask_Online(Squad, Success);
			break;
		}
	case ESimulationLevels::Buffered:
		{
			FinishTask_Buffer(Squad, Success);
			break;
		}
	case ESimulationLevels::Offline:
		{
			FinishTask_Offline(Squad, Success);
			break;
		}
	}
}

bool USquadTaskBase::CheckTaskFinished(UAISimProfileSquad* Squad)
{
	if(CheckTaskFinished_Always(Squad))
	{
		return true;
	}
	switch (Squad->GetSimLevel())
	{
	case ESimulationLevels::Online:
		{
			return CheckTaskFinished_Online(Squad);
		}
	case ESimulationLevels::Buffered:
		{
			return CheckTaskFinished_Buffer(Squad);
		}
	case ESimulationLevels::Offline:
		{
			return CheckTaskFinished_Offline(Squad);
		}
	}
	return true;
}

void USquadTaskBase::Save_Implementation(FSerializedProfileView Data)
{
	Data.GetElem().ObjectClass = GetClass();
	USimulationFunctionLibrary::SaveObjectData(this, Data.GetElem().ObjectData);
}

void USquadTaskBase::AbortTask_Implementation(UAISimProfileSquad* Squad)
{
	Aborted = true;
}

bool USquadTaskBase::CheckTaskFinished_Buffer_Implementation(UAISimProfileSquad* Squad)
{
	return true;
}

void USquadTaskBase::FinishTask_Buffer_Implementation(UAISimProfileSquad* Squad, bool Success)
{
}

void USquadTaskBase::ExecuteTask_Buffer_Implementation(UAISimProfileSquad* Squad, float DeltaTime)
{
}

void USquadTaskBase::BeginTask_Buffer_Implementation(UAISimProfileSquad* Squad)
{
}

bool USquadTaskBase::CheckTaskFinished_Offline_Implementation(UAISimProfileSquad* Squad)
{
	return true;
}

void USquadTaskBase::FinishTask_Offline_Implementation(UAISimProfileSquad* Squad, bool Success)
{
}

void USquadTaskBase::ExecuteTask_Offline_Implementation(UAISimProfileSquad* Squad, float DeltaTime)
{
}

void USquadTaskBase::BeginTask_Offline_Implementation(UAISimProfileSquad* Squad)
{
}

bool USquadTaskBase::CheckTaskFinished_Online_Implementation(UAISimProfileSquad* Squad)
{
	return true;
}

void USquadTaskBase::FinishTask_Online_Implementation(UAISimProfileSquad* Squad, bool Success)
{
}

void USquadTaskBase::ExecuteTask_Online_Implementation(UAISimProfileSquad* Squad, float DeltaTime)
{
}

void USquadTaskBase::BeginTask_Online_Implementation(UAISimProfileSquad* Squad)
{
}

bool USquadTaskBase::CheckTaskFinished_Always_Implementation(UAISimProfileSquad* Squad)
{
	return Aborted;
}

void USquadTaskBase::FinishTask_Always_Implementation(UAISimProfileSquad* Squad, bool Success)
{
}

void USquadTaskBase::ExecuteTask_Always_Implementation(UAISimProfileSquad* Squad, float DeltaTime)
{
}

void USquadTaskBase::BeginTask_Always_Implementation(UAISimProfileSquad* Squad)
{
}
