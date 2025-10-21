// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationBaseTrait.h"

#include "GraphPositionFragment.h"
#include "MassEntityTemplateRegistry.h"

void USimulationBaseTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	Super::BuildTemplate(BuildContext, World);
	BuildContext.AddFragment<FGraphPositionFragment>();
}
