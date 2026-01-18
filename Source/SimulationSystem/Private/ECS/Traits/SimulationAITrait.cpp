// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationAITrait.h"

#include "AIFragment.h"
#include "MassEntityTemplateRegistry.h"

void USimulationAITrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FAIFragment>();
}
