// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED_SimulationSquadTrait.h"

#include "GraphOfflineSpeedFragment.h"
#include "GraphTargetPositionFragment.h"
#include "MassEntityTemplateRegistry.h"
#include "SquadTag.h"

void UDEPRECATED_SimulationSquadTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	Super::BuildTemplate(BuildContext, World);
	BuildContext.AddTag<FSquadTag>();
	BuildContext.AddFragment<FGraphOfflineSpeedFragment>();
	BuildContext.AddFragment<FGraphTargetPositionFragment>();
}
