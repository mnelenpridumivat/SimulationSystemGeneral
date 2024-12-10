// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimProfileItem.h"
#include "SimProfileStackable.h"
#include "SimProfileItemStack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SIMULATIONSYSTEM_API USimProfileItemStack : public USimProfileItem, public ISimProfileStackable
{
public:
	virtual void CombineStacks_Implementation(USimProfileBase* Other) override;
	virtual int GetMaxStackSize_Implementation() const override;
	virtual int GetStackSize_Implementation() const override;
	virtual bool IsValidStack_Implementation() const override;
	virtual USimProfileBase* SplitFromStack_Implementation(int Num) override;
	virtual void SetStackSize(int NewSize) override;
	virtual void SetMaxStackSize(int NewSize) override;

private:
	GENERATED_BODY()

protected:
	
	UPROPERTY()
	int CurrentStackSize = 1;
	
	UPROPERTY()
	int MaxStackSize = 1;
	
};
