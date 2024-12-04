// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimVertexID.h"
#include "EditorSimActor.generated.h"

class USimProfileBase;

// This class does not need to be modified.
UINTERFACE()
class SIMULATIONSYSTEM_API UEditorSimActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMULATIONSYSTEM_API IEditorSimActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual USimProfileBase* GetProfile(){return nullptr;}
	virtual void DirectSetGraphVertex(const FSimVertexID& SimVertexID){}
	
};
