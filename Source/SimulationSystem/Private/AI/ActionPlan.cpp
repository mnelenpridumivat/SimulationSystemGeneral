// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlan.h"

void UActionPlan::SetActions(const TArray<UAction*>& Actions)
{
	PlanActions = Actions;
}
