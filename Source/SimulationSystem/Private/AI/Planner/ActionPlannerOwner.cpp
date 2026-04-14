// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlannerOwner.h"

#include "MassEntitySubsystem.h"

void UActionPlannerOwner::SetObjectOwner(UObject* NewObjectOwner)
{
	if (!ensureMsgf(
		IsValid(NewObjectOwner),
		TEXT("Attempt to set invalid ObjectOwner in ActionPlannerOwner [%s]"),
		*GetName()))
	{
		return;
	}
	ensureMsgf(
		!IsValid(ObjectOwner),
		TEXT("Attempt to set ObjectOwner [%s] to ActionPlannerOwner [%s] while other ObjectOwner [%s] is already set!"),
		*(NewObjectOwner->GetName()),
		*GetName(),
		*(ObjectOwner->GetName()));
	ensureMsgf(
		!EntityOwner.IsValid(),
		TEXT("Attempt to set ObjectOwner [%s] to ActionPlannerOwner [%s] while other EntityOwner is already set!"),
		*(NewObjectOwner->GetName()),
		*GetName());
	ObjectOwner = NewObjectOwner;
}

void UActionPlannerOwner::SetEntityOwner(FMassEntityHandle NewEntityOwner)
{
	if (!ensureMsgf(
		NewEntityOwner.IsValid(),
		TEXT("Attempt to set invalid EntityOwner in ActionPlannerOwner [%s]"),
		*GetName()))
	{
		return;
	}
	ensureMsgf(
		!EntityOwner.IsValid(),
		TEXT("Attempt to set EntityOwner to ActionPlannerOwner [%s] while other EntityOwner is already set!"),
		*GetName());
	ensureMsgf(
		!IsValid(ObjectOwner),
		TEXT("Attempt to set EntityOwner to ActionPlannerOwner [%s] while other ObjectOwner [%s] is already set!"),
		*GetName(),
		*(ObjectOwner->GetName()));
	EntityOwner = NewEntityOwner;
}

ESimualtionSystemProfileType UActionPlannerOwner::GetProfileType() const
{
	switch (GetSimulationSystemProfileType())
	{
	case ESimualtionSystemProfileType::Classes:
		{
			if (IsValid(ObjectOwner))
			{
				return ESimualtionSystemProfileType::Classes;
			}
			break;
		}
	case ESimualtionSystemProfileType::ECS:
		{
			if (EntityOwner.IsValid())
			{
				return ESimualtionSystemProfileType::ECS;
			}
			break;
		}
	default:
		{
			ensure(false);
		}
	}
	return ESimualtionSystemProfileType::MAX;
}

bool UActionPlannerOwner::GetFragment(UScriptStruct* Struct, FFragmentProperties& Out)
{
	if (!ensure(GetProfileType() == ESimualtionSystemProfileType::ECS))
	{
		return false;
	}
	auto Subsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	auto& Manager = Subsystem->GetEntityManager();
	auto View = Manager.GetFragmentDataStruct(EntityOwner, Struct);

	if (!ensure(View.IsValid()))
	{
		return false;
	}

	Out.Struct.InitializeAs(Struct, View.GetMemory());
	for (TFieldIterator<FProperty> Iterator(Struct); Iterator; ++Iterator)
	{
		auto Property = *Iterator;
		if (Property->HasAnyPropertyFlags(CPF_BlueprintVisible))
		{
			Out.Props.Add(*Iterator);
		}
	}
	
	return true;
}

FMassEntityHandle UActionPlannerOwner::GetEntityOwner() const
{
	if (!ensure(EntityOwner.IsValid()))
	{
		return FMassEntityHandle();
	}
	return EntityOwner;
}

UObject* UActionPlannerOwner::GetObjectOwner() const
{
	if (!ensure(IsValid(ObjectOwner)))
	{
		return nullptr;
	}
	return ObjectOwner;
}
