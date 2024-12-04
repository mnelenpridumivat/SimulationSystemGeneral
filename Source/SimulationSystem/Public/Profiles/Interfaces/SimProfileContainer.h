// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimProfileContainer.generated.h"

class USimProfileBase;
// This class does not need to be modified.
UINTERFACE()
class SIMULATIONSYSTEM_API USimProfileContainer : public UInterface
{
	GENERATED_BODY()
};


/**
 * Interface for profiles which represents a container of other items (bags, squads, etc)
 * Do not use with SimProfileStackable interface!
 */
class SIMULATIONSYSTEM_API ISimProfileContainer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanStoreItem(USimProfileBase* Profile);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddItem(USimProfileBase* Profile);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveItem(USimProfileBase* Profile);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HasItem(USimProfileBase* Profile);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<USimProfileBase*> GetAllItems();
	
};
