// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlanner.h"

#include "Action.h"
#include "ActionPlan.h"
#include "Evaluator.h"
#include "TPriorityQueue.h"

void FActionStorage::SetState(FName Key, bool Value)
{
	State.FindOrAdd(Key) = Value;
}

bool FActionStorage::GetState(FName Key) const
{
	if (ensureMsgf(
		State.Contains(Key),
		TEXT("State [%s] do not exists!"),
		*Key.ToString()))
	{
		return State[Key];
	}
	return false;
}

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

UObject* UActionPlanner::GetParentObject()
{
	if (!ensureMsgf(
		IsValid(ParentObject),
		TEXT("Attempt to get invalid ParentObject in ActionPlanner [%s]"),
		*GetName()))
	{
		return nullptr;
	}
	return ParentObject;
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
				NewEvaluator->SetParentPlanner(this);
			}
		}
	}
	for (auto Action : Actions)
	{
		if (ensureMsgf(
			IsValid(Action.ActionClass),
			TEXT("In ActionPlanner [%s] (Object [%s]) there is an invalid Action!"),
			*GetName(),
			*(ParentObject->GetName())))
		{
			if (auto NewAction = NewObject<UAction>(GetWorld(), Action.ActionClass);
				ensureMsgf(
				IsValid(NewAction),
				TEXT("Unable to spawn action [%s] in ActionPlanner [%s] (Object [%s])!"),
				*(Action.ActionClass->GetName()),
				*GetName(),
				*(ParentObject->GetName()))			)
			{
				NewAction->SetParentPlanner(this);
				ensureMsgf(
					Action.Weight > 0.0f,
					TEXT("Action [%s] has strange Weight [%f] - can potentially cause strange results."),
					*Action.ActionClass->GetName(),
					Action.Weight);
				ActionsObjs.Add({NewAction, Action.Weight});
				NewAction->BeginPlay();
			}
		}
	}
}

UActionPlan* UActionPlanner::MakeDecision(const FActionPlannerGoal& Goal)
{
	/*
	 * Create guard for IsThinking variable to set it to true while making decision and forget about it's existence.
	 * Then function reaches end, IsThinking will automatically set to false
	 */
	struct ThinkingGuard
	{
		ThinkingGuard(bool& IsThinkingVar){IsThinkingPtr = &IsThinkingVar; IsThinkingVar = true;}
		~ThinkingGuard() { *IsThinkingPtr = false;}

	private:
		bool* IsThinkingPtr;
	};
	ThinkingGuard Guard(IsThinking);
	
	// Update anything we know about world
	for (auto Evaluator : EvaluatorsObjs)
	{
		Evaluator->Evaluate();
	}

	// Verify if we actually need to do something or we already on goal
	{
		bool IsOnGoal = true;
		for (auto& elem : Goal.State)
		{
			if (ActionStorage.GetState(elem.Key) != elem.Value)
			{
				IsOnGoal = false;
				break;
			}
		}
		if (IsOnGoal)
		{
			return NewObject<UActionPlan>(GetWorld());
		}
	}
	
	struct Variant
	{
		/* 
		 * First action - action to reach goal
		 * Last action - first action to execute
		 */
		TArray<UAction*> Actions;
		/*
		 * State of known info at last action in array
		 */
		FActionPlannerGoal CurrentState;
	};
	TPriorityQueue<Variant> PriorityQueue;

	auto VerifyActionsFunc = [&](const float PrevWeight, const TArray<UAction*>& PrevActions, const FActionPlannerGoal& LocGoal)
	{
		bool NeedToSearchDeeper = false;
		for (auto& elem : LocGoal.State)
		{
			if (ActionStorage.GetState(elem.Key) != elem.Value)
			{
				NeedToSearchDeeper = true;
				break;
			}
		}
		if (NeedToSearchDeeper)
		{
			for (auto [Action, Weight] : ActionsObjs)
			{
				if (Action->CheckIfWeCanCreateState(LocGoal))
				{
					Variant NewVariant;
					NewVariant.Actions = PrevActions;
					NewVariant.CurrentState = LocGoal;
					NewVariant.Actions.Add(Action);
					Action->GetRequiredStartGoal(NewVariant.CurrentState);
					PriorityQueue.Push(NewVariant, PrevWeight+Weight);
				}
			}
		}
		return !NeedToSearchDeeper;
	}; // return true if passed PrevActions are the working plan
	
	VerifyActionsFunc(0, {}, Goal);
	
	while (!PriorityQueue.IsEmpty())
	{
		auto TopNode = PriorityQueue.PopNode();
		if (VerifyActionsFunc(TopNode.Priority, TopNode.Element.Actions, TopNode.Element.CurrentState))
		{
			Plan = NewObject<UActionPlan>(GetWorld());
			Plan->SetActions(TopNode.Element.Actions);
			return Plan;
		}
	}

	return nullptr;
}
