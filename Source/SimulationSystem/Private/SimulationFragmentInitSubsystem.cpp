// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationFragmentInitSubsystem.h"

void USimulationFragmentInitSubsystem::CallInitIfCan(FEntityFragmentKey Entity)
{
	auto FuncPtr = DefferedInits.Find(Entity);
	if (!FuncPtr)
	{
		return;
	}
	(*FuncPtr)(Entity.Entity);
	DefferedInits.Remove(Entity);
}

void USimulationFragmentInitSubsystem::BindDefferedInit(FEntityFragmentKey Key, InitFunc Func)
{
	ensure(!DefferedInits.Contains(Key));
	DefferedInits.Add(Key, Func);
}
