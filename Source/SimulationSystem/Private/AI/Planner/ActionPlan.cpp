// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlan.h"

#include "Action.h"

void UActionPlan::SetActions(const TArray<UAction*>& Actions)
{
	PlanActions = Actions;
}

void UActionPlan::Execute()
{
	if(!CanExecute())
	{
		return;
	}
	if(ensure(PlanActions.IsValidIndex(CurrentStep)))
	{
		bCanExecute = false;
		return;
	}
	auto CurrentAction = PlanActions[CurrentStep];
	if(!ensure(IsValid(CurrentAction)))
	{
		bCanExecute = false;
		return;
	}
	if(HasStepChanged)
	{
		CurrentAction->NativeStartAction();
		HasStepChanged = false;
	}
	const auto ActionStatus = CurrentAction->NativeProcessAction();
	switch(ActionStatus)
	{
	case EActionStatus::Running:
		{
			break;
		}
	case EActionStatus::Aborted:
	case EActionStatus::Finished:
		{
			CurrentAction->NativeEndAction(ActionStatus == EActionStatus::Finished);
			CurrentStep++;
			HasStepChanged = true;
			if(CurrentStep >= PlanActions.Num())
			{
				bCanExecute = false;
			}
			break;
		}
	default:
		{
			ensureMsgf(
				false,
				TEXT("After Action [%s] execution it has invalid status [%s]"),
				*CurrentAction->GetName(),
				*UEnum::GetValueAsString(ActionStatus));
			bCanExecute = false;
		};
	}
	
}
