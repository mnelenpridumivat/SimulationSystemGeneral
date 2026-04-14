// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffect.h"

#include "Action.h"

void UActionEffect::SetParentAction(UAction* Action)
{
	if (!ensureMsgf(
		IsValid(Action),
		TEXT("Attempt to set invalid ParentAction in Effect [%s]"),
		*GetName()))
	{
		return;
	}
	ensureMsgf(
		!IsValid(ParentAction),
		TEXT("Attempt to set ParentAction [%s] in Effect [%s] while other ParentAction [%s] is already set!"),
		*(Action->GetName()),
		*GetName(),
		*(ParentAction->GetName()));
	ParentAction = Action;
}
