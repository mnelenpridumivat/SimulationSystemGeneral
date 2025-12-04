// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationOnlineSpawnableTrait.h"
#include "MassEntityTemplateRegistry.h"
#include "OnlineActorClassFragment.h"
#include "OnlineActorFragment.h"

void USimulationOnlineSpawnableTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext,
                                                    const UWorld& World) const
{
	//Super::BuildTemplate(BuildContext, World);
	BuildContext.AddFragment<FOnlineActorClassFragment>();
	BuildContext.AddFragment<FOnlineActorFragment>();
}
