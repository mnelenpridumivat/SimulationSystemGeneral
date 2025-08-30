// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSystemSettings.h"

#if WITH_EDITOR
void USimulationSystemSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Characteristics
	if (PropertyName == GET_MEMBER_NAME_CHECKED(USimulationSystemSettings, CharacteristicLevelMin)
		|| PropertyName == GET_MEMBER_NAME_CHECKED(USimulationSystemSettings, CharacteristicLevelMax))
	{
		if(CharacteristicLevelMax <= CharacteristicLevelMin)
		{
			CharacteristicLevelMax = CharacteristicLevelMin+1;
		}
		for(auto& elem : CharacteristicsList)
		{
			if(elem.Value > CharacteristicLevelMax)
			{
				elem.Value = CharacteristicLevelMax;
			}
			if(elem.Value < CharacteristicLevelMin)
			{
				elem.Value = CharacteristicLevelMin;
			}
		}
	}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(USimulationSystemSettings, PointsLevelMax)
		|| PropertyName == GET_MEMBER_NAME_CHECKED(USimulationSystemSettings, PointsLevelMin))
	{
		if(PointsLevelMax <= PointsLevelMin)
		{
			PointsLevelMax = PointsLevelMin+1;
		}
		for(auto& elem : PointsList)
		{
			if(elem.Value > PointsLevelMax)
			{
				elem.Value = PointsLevelMax;
			}
			if(elem.Value < PointsLevelMin)
			{
				elem.Value = PointsLevelMin;
			}
		}
	}

	// Functions
	if (PropertyName == GET_MEMBER_NAME_CHECKED(USimulationSystemSettings, SimulationSystemFunctions))
	{
		OnFunctionsPropertyChanged.Broadcast();
	}
}
#endif
