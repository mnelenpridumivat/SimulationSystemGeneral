// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED_SimulationSmartTrait.h"

#include "MassEntityTemplateRegistry.h"
#include "SmartTag.h"

void UDEPRECATED_SimulationSmartTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	Super::BuildTemplate(BuildContext, World);
	BuildContext.AddTag<FSmartTag>();
}
