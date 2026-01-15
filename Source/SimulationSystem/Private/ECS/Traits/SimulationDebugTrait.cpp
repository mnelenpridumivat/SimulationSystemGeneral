// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationDebugTrait.h"

#include "DebugFragment.h"
#include "MassEntityTemplateRegistry.h"

void USimulationDebugTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
#if !UE_BUILD_SHIPPING
	auto& DebugFragment = BuildContext.AddFragment_GetRef<FDebugFragment>();
	ensure(Archetype);
	DebugFragment.Archetype = Archetype;
#endif
}
