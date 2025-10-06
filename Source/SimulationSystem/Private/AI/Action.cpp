// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"

#include "ActionEffect.h"
#include "ActionPlanner.h"
#include "ActionPrecondition.h"

void UAction::SetParentPlanner(UActionPlanner* Planner)
{
	if (!ensureMsgf(
		IsValid(Planner),
		TEXT("Attempt to set invalid ParentPlanner for action [%s]"),
		*GetName()))
	{
		return;
	}
	ensureMsgf(
		!IsValid(ParentPlanner),
		TEXT("Attempt to set ActionPlanner [%s] in Action [%s] while other ActionPlanner [%s] is already set!"),
		*(Planner->GetName()),
		*GetName(),
		*(ParentPlanner->GetName()));
	ParentPlanner = Planner;
}

void UAction::BeginPlay()
{
	if (!ensureMsgf(
		IsValid(ParentPlanner),
		TEXT("Action [%s] has invalid ParentPlanner!"),
		*GetName()))
	{
		return;
	}
	for (auto Precondition : Preconditions)
	{
		if (ensureMsgf(
			IsValid(Precondition),
			TEXT("Action [%s] has invalid Precondition!"),
			*GetName()))
		{
			if (auto NewPrecondition = NewObject<UActionPrecondition>(GetWorld(), Precondition);
				ensureMsgf(
				IsValid(NewPrecondition),
				TEXT("Unable to spawn Precondition [%s] in Action [%s]!"),
				*(Precondition->GetName()),
				*GetName())			)
			{
				NewPrecondition->SetParentAction(this);
				PreconditionsObjs.Add(NewPrecondition);
			}
		}
	}
	for (auto Effect : Effects)
	{
		if (ensureMsgf(
			IsValid(Effect),
			TEXT("Action [%s] has invalid Effect!"),
			*GetName()))
		{
			if (auto NewEffect = NewObject<UActionEffect>(GetWorld(), Effect);
				ensureMsgf(
				IsValid(NewEffect),
				TEXT("Unable to spawn Effect [%s] in Action [%s]!"),
				*(Effect->GetName()),
				*GetName())			)
			{
				NewEffect->SetParentAction(this);
				EffectsObjs.Add(NewEffect);
			}
		}
	}
}

bool UAction::CheckPreconditions(const FActionStorage& Storage)
{
	for (auto Precondition : PreconditionsObjs)
	{
		if (Storage.GetState(Precondition->GetKey()) != Precondition->GetValue())
		{
			return false;
		}
	}
	return true;
}

bool UAction::CheckSelfPreconditions()
{
	if (!ensureMsgf(
		IsValid(ParentPlanner),
		TEXT("Action [%s] has invalid ParentPlanner!"),
		*GetName()))
	{
		return false;
	}
	return CheckPreconditions(ParentPlanner->GetActionStorage());
}

bool UAction::CheckIfWeCanCreateState(const FActionPlannerGoal& Storage)
{
	auto Copy = Storage;
	for (auto Effect : EffectsObjs)
	{
		if (Copy.State.Contains(Effect->GetKey()))
		{
			Copy.State[Effect->GetKey()] = Effect->GetValue();
		}
	}
	for (auto& elem : Copy.State)
	{
		if (Storage.State[elem.Key] != elem.Value)
		{
			return false;
		}
	}
	return true;
}

void UAction::GetRequiredStartGoal(FActionPlannerGoal& Storage)
{
	for (auto Precondition : PreconditionsObjs)
	{
		Storage.State.FindOrAdd(Precondition->GetKey()) = Precondition->GetValue();
	}
}
