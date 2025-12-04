// Fill out your copyright notice in the Description page of Project Settings.


#include "Properties/ProfileGenerator.h"

#include "SimProfileBase.h"
#include "SimProfileContainer.h"
#include "SimProfileStackable.h"


USimProfileBase* UProfileGenerator::GenerateProfileOld()
{
	ensureMsgf(false, TEXT("GenerateProfileOld method is not overriden or called UProfileGenerator::GenerateProfileOld!"));
	return nullptr;
}

FMassEntityHandle UProfileGenerator::GenerateProfile()
{
	ensureMsgf(false, TEXT("GenerateProfile method is not overriden or called UProfileGenerator::GenerateProfile!"));
	return FMassEntityHandle();
}
