// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimulationSystemDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Engine, defaultconfig)
class SIMULATIONSYSTEMDEVELOPERTOOL_API USimulationSystemDeveloperSettings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(config, EditAnywhere, Category="SystemGenerationTools")
	TEnumAsByte<ECollisionChannel> SimPointsTraceChannel;

	UPROPERTY(config, EditAnywhere, Category="SystemGenerationTools")
	float MinimumDistance = 500.0f;

	UPROPERTY(config, EditAnywhere, Category="SystemGenerationTools")
	float DefaultDistance = 5000.0f;

	UPROPERTY(config, EditAnywhere, Category="SystemGenerationTools")
	float MaximumDistance = 7000.0f;

	UPROPERTY(config, EditAnywhere, Category="SystemGenerationTools")
	float PointZOffset = 50.0f;

	UPROPERTY(config, EditAnywhere, Category="GraphPointShape")
	TSoftObjectPtr<UStaticMesh> ShapeMesh;

	UPROPERTY(config, EditAnywhere, Category="GraphPointShape")
	TSoftObjectPtr<UMaterialInterface> ShapeMaterial;
	
	UPROPERTY(config, EditAnywhere, Category="DebugDraw")
	float ShowTime = 30.0f;
	
	UPROPERTY(config, EditAnywhere, Category="DebugDraw")
	float LineThickness = 10.0f;
	
	UPROPERTY(config, EditAnywhere, Category="DebugDraw")
	FColor LineColor = FColor::Blue;
};
