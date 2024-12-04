// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskDebug/TaskDebugBase.h"
#include "TaskDebugMoveToCamp.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEMDEVELOPERTOOL_API UTaskDebugMoveToCamp : public UTaskDebugBase
{
	GENERATED_BODY()

public:

	virtual void VisualizeTaskInfo(float DeltaTime, UAISimProfileSquad* Squad, USquadTaskBase* Task) override;
	virtual void ImGuiTaskInfo(float DeltaTime, UAISimProfileSquad* Squad, USquadTaskBase* Task) override;
};
