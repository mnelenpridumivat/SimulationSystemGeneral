#pragma once

#include "CoreMinimal.h"
#include "Characteristic.h"
#include "ProfileGeneratorItem.h"
#include "SimulationSystemSettings.h"
#include "Engine/DataTable.h"
#include "PawnProperty.generated.h"

class USimulationSystemSettings;

USTRUCT()
struct FPawnProperty : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize)
	TMap<FName, int> Characteristics;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize)
	TMap<FName, float> Points;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FProfileGeneratorItem> Items;

	FPawnProperty(){
		auto Settings = GetMutableDefault<USimulationSystemSettings>();
		for(auto& elem : Settings->CharacteristicsList)
		{
			Characteristics.Add(elem.Key, elem.Value);
		}
		for(auto& elem : Settings->PointsList)
		{
			Points.Add(elem.Key, elem.Value);
		}
	}
	
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
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
	}
	
};
