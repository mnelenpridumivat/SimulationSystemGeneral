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
	auto& CharacteristicsList = USimulationSystemSettings::StaticClass()->GetDefaultObject<USimulationSystemSettings>()->CharacteristicsList;
	auto& PointsList = USimulationSystemSettings::StaticClass()->GetDefaultObject<USimulationSystemSettings>()->PointsList;
	for(auto& Characteristic : CharacteristicsList)
	{
		Characteristics.Add(Characteristic);
	}
	for(auto& Point : PointsList)
	{
		Points.Add(Point);
	}
}
