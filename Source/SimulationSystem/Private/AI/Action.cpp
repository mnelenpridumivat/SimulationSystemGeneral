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
	for (const auto& Precondition : Preconditions)
	{
		auto NewPrecondition = NewObject<UActionPrecondition>(GetWorld());
		NewPrecondition->SetKey(Precondition.Key);
		NewPrecondition->SetValue(Precondition.Value);
		NewPrecondition->SetParentAction(this);
		PreconditionsObjs.Add(NewPrecondition);
	}
	for (const auto& Effect : Effects)
	{
		auto NewEffect = NewObject<UActionEffect>(GetWorld());
		NewEffect->SetKey(Effect.Key);
		NewEffect->SetValue(Effect.Value);
		NewEffect->SetParentAction(this);
		EffectsObjs.Add(NewEffect);
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
	bool NoMatch = true;
	for (auto Effect : EffectsObjs)
	{
		if (Copy.State.Contains(Effect->GetKey()))
		{
			Copy.State[Effect->GetKey()] = Effect->GetValue();
			NoMatch = false;
		}
	}
	if (NoMatch)
	{
		return false;
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

UObject* UAction::GetOwningObject() const
{
	if(!ensureMsgf(
		IsValid(ParentPlanner),
		TEXT("Attempt to retrieve owning object for action [%s] while ParentPlanner is invalid!"),
		*GetName()))
	{
		return nullptr;
	}
	return ParentPlanner->GetParentObject();
}

void UAction::FinishAction()
{
	if (Status == EActionStatus::Running)
	{
		Status = EActionStatus::Finished;
	}
}

void UAction::AbortAction()
{
	if(Status == EActionStatus::Running)
	{
		Status = EActionStatus::Aborted;
	}
}

void UAction::StartAction_Implementation()
{
}

void UAction::NativeStartAction()
{
	if(ensureMsgf(
		Status == EActionStatus::Idle,
		TEXT("Attempt to start Action [%s] with status [%s]!"),
		*GetName(),
		*UEnum::GetValueAsString(Status)))
	{
		AbortAction();
	}
	Status = EActionStatus::Running;
	StartAction();
}

void UAction::ProcessAction_Implementation()
{
}

EActionStatus UAction::NativeProcessAction()
{
	if (Status == EActionStatus::Aborted || Status == EActionStatus::Finished)
	{
		return Status;
	}
	ProcessAction();
	if(ensureMsgf(
		Status != EActionStatus::Idle,
		TEXT("In Action [%s] (OwningObject [%s]) status is Idle after ProcessAction!"),
		*GetName(),
		GetOwningObject() ? *(GetOwningObject()->GetName()) : TEXT("Invalid")))
	{
		return EActionStatus::Aborted;
	}
	return Status;
}

void UAction::EndAction_Implementation(bool Successful)
{
}

void UAction::NativeEndAction(bool Successful)
{
	EndAction(Successful);
	Status = EActionStatus::Idle;
}
