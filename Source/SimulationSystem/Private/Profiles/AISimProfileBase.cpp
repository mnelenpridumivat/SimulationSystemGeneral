// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Profiles\AISimProfileBase.h"

#include "SimulationSystemSettings.h"

void UAISimProfileBase::NativeOnRegistered()
{
	Super::NativeOnRegistered();
}

void UAISimProfileBase::NativeOnCreated()
{
	Super::NativeOnCreated();
	auto& CharacteristicsListRef = USimulationSystemSettings::StaticClass()->GetDefaultObject<USimulationSystemSettings>()->CharacteristicsList;
	auto& PointsListRef = USimulationSystemSettings::StaticClass()->GetDefaultObject<USimulationSystemSettings>()->PointsList;
	for(auto& Characteristic : CharacteristicsListRef)
	{
		CharacteristicsList.Add(Characteristic);
	}
	for(auto& Point : PointsListRef)
	{
		PointsList.Add(Point);
	}
}
