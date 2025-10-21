// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSmartTrait.h"

#include "MassEntityTemplateRegistry.h"
#include "SmartTag.h"

void USimulationSmartTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	Super::BuildTemplate(BuildContext, World);
	BuildContext.AddTag<FSmartTag>();
}
