// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MassEntityTraitBase.h"

#include "GraphMovingTrait.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEMECS_API UGraphMovingTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

public:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
