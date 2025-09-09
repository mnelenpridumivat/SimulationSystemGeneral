// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Profiles\AISimProfileBase.h"

#include "SimulationSystemSettings.h"

void UAISimProfileBase::OnRegistered_Implementation()
{
	Super::OnRegistered_Implementation();
}

void UAISimProfileBase::OnCreated_Implementation()
{
	Super::OnCreated_Implementation();
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
