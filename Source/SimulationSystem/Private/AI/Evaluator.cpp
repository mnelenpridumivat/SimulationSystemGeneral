// Fill out your copyright notice in the Description page of Project Settings.


#include "Evaluator.h"

#include "ActionPlanner.h"

void UEvaluator::SetParentPlanner(UActionPlanner* Planner)
{
	if (!ensureMsgf(
		IsValid(Planner),
		TEXT("Attempt to set invalid ParentPlanner for evaluator [%s]"),
		*GetName()))
	{
		return;
	}
	ensureMsgf(
		!IsValid(ParentPlanner),
		TEXT("Attempt to set ActionPlanner [%s] in Evaluator [%s] while other ActionPlanner [%s] is already set!"),
		*(Planner->GetName()),
		*GetName(),
		*(ParentPlanner->GetName()));
	ParentPlanner = Planner;
}

void UEvaluator::SetState(FName key, bool value) const
{
	if (!ensureMsgf(
		IsValid(ParentPlanner),
		TEXT("Unable to set key [%s] in [%s] because ParentPlanner is invalid!"),
		*key.ToString(),
		*GetName()))
	{
		return;
	}
	ParentPlanner->GetActionStorage().SetState(key, value);
}

void UEvaluator::Evaluate_Implementation()
{
	ensureMsgf(
		false,
		TEXT("Did you forget to override Evaluate function in [%s]? Do not call Super if Super == UEvaluator!"),
		*GetName());
}
