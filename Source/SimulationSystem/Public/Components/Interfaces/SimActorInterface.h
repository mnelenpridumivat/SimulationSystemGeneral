// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimVertexID.h"
#include "UObject/Interface.h"
#include "SimActorInterface.generated.h"

class UProfileComponent;
// This class does not need to be modified.
UINTERFACE()
class SIMULATIONSYSTEM_API USimActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMULATIONSYSTEM_API ISimActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UProfileComponent* GetProfileComponent();

	virtual bool UseInSimulation(){return true;}
	virtual void DirectSetGraphVertex(const FSimVertexID& SimVertexID){}
	
};
