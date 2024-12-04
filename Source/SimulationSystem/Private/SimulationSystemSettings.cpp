// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSystemSettings.h"

#if WITH_EDITOR
void USimulationSystemSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);
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
#endif
