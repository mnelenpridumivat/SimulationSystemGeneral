// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED_SimulationPawnTrait.h"

#include "GraphOfflineSpeedFragment.h"
#include "GraphTargetPositionFragment.h"
#include "MassEntityTemplateRegistry.h"
#include "NPCTag.h"

void UDEPRECATED_SimulationPawnTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	Super::BuildTemplate(BuildContext, World);
	BuildContext.AddFragment<FGraphOfflineSpeedFragment>();
	BuildContext.AddFragment<FGraphTargetPositionFragment>();
}
