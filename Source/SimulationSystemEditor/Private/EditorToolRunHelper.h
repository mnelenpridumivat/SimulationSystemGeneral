// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EditorToolRunHelper.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEMEDITOR_API UEditorToolRunHelper : public UObject
{
	GENERATED_BODY()
public:
	TFunction<void()> OnDelay;

	UFUNCTION()
	void OnDelayComplete();
};
