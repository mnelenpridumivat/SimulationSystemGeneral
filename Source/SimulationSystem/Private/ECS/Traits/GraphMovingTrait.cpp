// Fill out your copyright notice in the Description page of Project Settings.


#include "Traits/GraphMovingTrait.h"

#include "GraphPositionFragment.h"
#include "MassEntityTemplateRegistry.h"

void UGraphMovingTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	Super::BuildTemplate(BuildContext, World);
	BuildContext.AddFragment<FGraphPositionFragment>();
}
