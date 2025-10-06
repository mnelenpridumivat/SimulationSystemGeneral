// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPrecondition.h"

#include "Action.h"

void UActionPrecondition::SetParentAction(UAction* Action)
{
	if (!ensureMsgf(
		IsValid(Action),
		TEXT("Attempt to set invalid ParentAction in Precondition [%s]"),
		*GetName()))
	{
		return;
	}
	ensureMsgf(
		!IsValid(ParentAction),
		TEXT("Attempt to set ParentAction [%s] in Precondition [%s] while other ParentAction [%s] is already set!"),
		*(Action->GetName()),
		*GetName(),
		*(ParentAction->GetName()));
	ParentAction = Action;
}
