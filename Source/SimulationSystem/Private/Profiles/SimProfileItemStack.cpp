// Fill out your copyright notice in the Description page of Project Settings.


#include "SimProfileItemStack.h"

void USimProfileItemStack::CombineStacks_Implementation(USimProfileBase* Other)
{
	ISimProfileStackable::CombineStacks_Implementation(Other);
}

int USimProfileItemStack::GetMaxStackSize_Implementation() const
{
	return MaxStackSize;
}

int USimProfileItemStack::GetStackSize_Implementation() const
{
	return CurrentStackSize;
}

bool USimProfileItemStack::IsValidStack_Implementation() const
{
	return CurrentStackSize > 0;
}

USimProfileBase* USimProfileItemStack::SplitFromStack_Implementation(int Num)
{
	return ISimProfileStackable::SplitFromStack_Implementation(Num);
}

void USimProfileItemStack::SetStackSize(int NewSize)
{
	CurrentStackSize = NewSize;
}

void USimProfileItemStack::SetMaxStackSize(int NewSize)
{
	MaxStackSize = NewSize;
}
