// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimProfileStackable.generated.h"

class USimProfileBase;
// This class does not need to be modified.
UINTERFACE()
class SIMULATIONSYSTEM_API USimProfileStackable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for profiles which represents a stack of items (ammunition, materials, etc).
 * Do not use with SimProfileContainer interface!
 */
class SIMULATIONSYSTEM_API ISimProfileStackable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int GetStackSize() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int GetMaxStackSize() const; 

	// Check if stack has valid num ([1,MaximumStackSize])
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsValidStack() const;

	/*
	 * Creates new stack profile and move passed amount from this. Always check if Num more than stack size!
	 * Returned profile does not registered in GlobalGraph (not locates on node or other profile)
	 * If need to move whole stack, move profile instead! 
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USimProfileBase* SplitFromStack(int Num);

	/*
	 * Increase stack amount from other stack. If sum bigger than limit, then left remainder in other stack.
	 * Always check other stack after combination and manually delete if necessary!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CombineStacks(USimProfileBase* Other);

	/*
	 * Only internal C++ usage!
	 */
	virtual void SetStackSize(int NewSize){};
	virtual void SetMaxStackSize(int NewSize){};
	
};
