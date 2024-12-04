// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TaskDebugBase.generated.h"

class UAISimProfileSquad;
class USquadTaskBase;
/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEMDEVELOPERTOOL_API UTaskDebugBase : public UObject
{
	GENERATED_BODY()

public:

	virtual void VisualizeTaskInfo(float DeltaTime, UAISimProfileSquad* Squad, USquadTaskBase* Task);
	virtual void ImGuiTaskInfo(float DeltaTime, UAISimProfileSquad* Squad, USquadTaskBase* Task);
	
};
