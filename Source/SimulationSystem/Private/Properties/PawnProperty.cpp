#include "..\..\Public\Properties\PawnProperty.h"

#include "SimulationSystemSettings.h"

/*FPawnProperty::FPawnProperty()
{
	auto Settings = GetMutableDefault<USimulationSystemSettings>();
	for(auto& elem : Settings->CharacteristicsList)
	{
		Characteristics.Add(elem.Key, elem.Value);
	}
	for(auto& elem : Settings->PointsList)
	{
		Points.Add(elem.Key, elem.Value);
	}
}*/

/*void FPawnProperty::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	FTableRowBase::OnDataTableChanged(InDataTable, InRowName);
	auto Settings = GetMutableDefault<USimulationSystemSettings>();
	
	TSet<FName> KeysToRemove;
	for(auto& elem : Settings->CharacteristicsList)
	{
		if(!Characteristics.Contains(elem.Key))
		{
			Characteristics.Add(elem.Key, elem.Value);
		}
	}
	for(auto& elem : Characteristics)
	{
		elem.Value = FMath::Clamp(elem.Value, Settings->CharacteristicLevelMin, Settings->CharacteristicLevelMax);
		if(!Settings->CharacteristicsList.Contains(elem.Key))
		{
			KeysToRemove.Add(elem.Key);
		}
	}
	for(auto& elem : KeysToRemove)
	{
		Characteristics.Remove(elem);
	}
	
	KeysToRemove.Empty();
	for(auto& elem : Settings->PointsList)
	{
		if(!Points.Contains(elem.Key))
		{
			Points.Add(elem.Key, elem.Value);
		}
	}
	for(auto& elem : Points)
	{
		elem.Value = FMath::Clamp(elem.Value, Settings->PointsLevelMin, Settings->PointsLevelMax);
		if(!Settings->PointsList.Contains(elem.Key))
		{
			KeysToRemove.Add(elem.Key);
		}
	}
	for(auto& elem : KeysToRemove)
	{
		Points.Remove(elem);
	}
}*/
