// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Action_MoveToRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UAction_MoveToRandomPoint : public UAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	float SearchRadius = 10000;

public:
	virtual void NativeStartAction(FMassCommandBuffer& CmdBuffer) override;
};
