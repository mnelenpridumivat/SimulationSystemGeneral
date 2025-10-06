// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlanner.h"

#include "Action.h"
#include "Evaluator.h"

void UActionPlanner::SetParentObject(UObject* Object)
{
	if (!ensureMsgf(
		IsValid(Object),
		TEXT("Attempt to set invalid ParentObject in ActionPlanner [%s]"),
		*GetName()))
	{
		return;
	}
	ensureMsgf(
		!IsValid(ParentObject),
		TEXT("Attempt to set ParentObject [%s] to ActionPlanner [%s] while other ParentObject [%s] is already set!"),
		*(Object->GetName()),
		*GetName(),
		*(ParentObject->GetName()));
	ParentObject = Object;
}

void UActionPlanner::BeginPlay()
{
	if (!ensureMsgf(IsValid(ParentObject), TEXT("ActionPlanner [%s] has invalid ParentObject!"), *GetName()))
	{
		return;
	}
	for (auto Evaluator : Evaluators)
	{
		if (ensureMsgf(
			IsValid(Evaluator),
			TEXT("In ActionPlanner [%s] (Object [%s]) there is an invalid Evaluator!"),
			*GetName(),
			*(ParentObject->GetName())))
		{
			if (auto NewEvaluator = NewObject<UEvaluator>(GetWorld(), Evaluator);
				ensureMsgf(
				IsValid(NewEvaluator),
				TEXT("Unable to spawn evaluator [%s] in ActionPlanner [%s] (Object [%s])!"),
				*(Evaluator->GetName()),
				*GetName(),
				*(ParentObject->GetName()))			)
			{
				EvaluatorsObjs.Add(NewEvaluator);
			}
		}
	}
	for (auto Action : Actions)
	{
		if (ensureMsgf(
			IsValid(Action),
			TEXT("In ActionPlanner [%s] (Object [%s]) there is an invalid Action!"),
			*GetName(),
			*(ParentObject->GetName())))
		{
			if (auto NewAction = NewObject<UAction>(GetWorld(), Action);
				ensureMsgf(
				IsValid(NewAction),
				TEXT("Unable to spawn action [%s] in ActionPlanner [%s] (Object [%s])!"),
				*(Action->GetName()),
				*GetName(),
				*(ParentObject->GetName()))			)
			{
				NewAction->SetParentPlanner(this);
				ActionsObjs.Add(NewAction);
				NewAction->BeginPlay();
			}
		}
	}
}
