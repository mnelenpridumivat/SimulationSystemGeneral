// Fill out your copyright notice in the Description page of Project Settings.


#include "Motivator.h"

#include "ActionPlanner.h"
#include "MotivatorConfigDataAsset.h"

void UMotivator::SetParentPlanner(UActionPlanner* Planner)
{
	if (!ensureMsgf(
		IsValid(Planner),
		TEXT("Attempt to set invalid ParentPlanner for motivator [%s]"),
		*GetName()))
	{
		return;
	}
	ensureMsgf(
		!IsValid(ParentPlanner),
		TEXT("Attempt to set ActionPlanner [%s] in Motivator [%s] while other ActionPlanner [%s] is already set!"),
		*(Planner->GetName()),
		*GetName(),
		*(ParentPlanner->GetName()));
	ParentPlanner = Planner;
}

bool UMotivator::NativeFindGoal(FActionPlannerGoal& Goal)
{
	bool NoNeed = true;
	if (IsValid(MotivatorSettings))
	{
		auto& Storage = ParentPlanner->GetActionStorage();
		NoNeed = UMotivatorConfigDataAsset::FindGoal(
			MotivatorSettings->GetDefaultObject<UMotivatorConfigDataAsset>(),
			Storage, Goal);
	} else {
		NoNeed = FindGoal(Goal);
	}
	ensureMsgf(
		NoNeed || !Goal.State.IsEmpty(),
		TEXT("No goal found in Motivator [%s]"),
		*GetName());
	return NoNeed;
}

bool UMotivator::FindGoal_Implementation(FActionPlannerGoal& Goal)
{
	return false;
}

UActionPlannerOwner* UMotivator::GetOwningObject() const
{
	if(!ensureMsgf(
		IsValid(ParentPlanner),
		TEXT("Attempt to retrieve owning object for motivator [%s] while ParentPlanner is invalid!"),
		*GetName()))
	{
		return nullptr;
	}
	return ParentPlanner->GetParentObject();
}

bool UMotivator::GetPlannerKey(FName Key)
{
	if(!ensureMsgf(
		IsValid(ParentPlanner),
		TEXT("Attempt to retrieve knowledge [%s] for motivator [%s] while ParentPlanner is invalid!"),
		*Key.ToString(),
		*GetName()))
	{
		return false;
	}
	return ParentPlanner->GetKey(Key);
}
