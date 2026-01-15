// Fill out your copyright notice in the Description page of Project Settings.


#include "Traits/SimulationGraphMovingTrait.h"

#include "GraphOfflineSpeedFragment.h"
#include "GraphTargetPositionFragment.h"
#include "MassEntityTemplateRegistry.h"

void USimulationGraphMovingTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	//Super::BuildTemplate(BuildContext, World);
	BuildContext.AddFragment<FGraphOfflineSpeedFragment>();
	//BuildContext.AddFragment<FGraphTargetPositionFragment>();
}
