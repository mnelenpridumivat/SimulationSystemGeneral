// Fill out your copyright notice in the Description page of Project Settings.


#include "Properties/ProfileGenerator.h"

#include "SimProfileBase.h"
#include "SimProfileContainer.h"
#include "SimProfileStackable.h"


USimProfileBase* UProfileGenerator::GenerateProfile()
{
	ensureMsgf(false, TEXT("GenerateProfile method is not overriden or called UProfileGenerator::GenerateProfile!"));
	return nullptr;
}
