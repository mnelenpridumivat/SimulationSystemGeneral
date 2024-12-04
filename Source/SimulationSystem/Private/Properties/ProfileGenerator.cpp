// Fill out your copyright notice in the Description page of Project Settings.


#include "Properties/ProfileGenerator.h"

#include "SimProfileBase.h"
#include "SimProfileContainer.h"
#include "SimProfileStackable.h"

/*bool UProfileGenerator::CanEditChange(const FProperty* InProperty) const
{
	if(!Super::CanEditChange(InProperty))
	{
		return false;
	}
	
	if(InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UProfileGenerator, Items))
	{
		auto Value = static_cast<const FClassProperty*>(
			GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UProfileGenerator, ProfileClass))
			)->GetPropertyValue_InContainer(this);
		if(!IsValid(Value))
		{
			return false;
		}
		return Value->GetClass()->ImplementsInterface(USimProfileContainer::StaticClass());
	}
	
	if(InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UProfileGenerator, AmountMin)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UProfileGenerator, AmountMax))
	{
		auto Value = static_cast<const FClassProperty*>(
			GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UProfileGenerator, ProfileClass))
			)->GetPropertyValue_InContainer(this);
		if(!IsValid(Value))
		{
			return false;
		}
		return Value->GetClass()->ImplementsInterface(USimProfileStackable::StaticClass());
	}
	
	return true;
}*/

USimProfileBase* UProfileGenerator::GenerateProfile()
{
	return NewObject<USimProfileBase>(ProfileClass);
}
