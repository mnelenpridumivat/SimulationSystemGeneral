// Fill out your copyright notice in the Description page of Project Settings.


#include "ProfileComponentCamp.h"

#include "SimProfileBase.h"
#include "SimProfileCamp.h"


// Sets default values for this component's properties
UProfileComponentCamp::UProfileComponentCamp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProfileComponentCamp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

USimProfileBase* UProfileComponentCamp::CreateNewProfile_Implementation()
{
	auto Profile = Super::CreateNewProfile_Implementation();
	ensureMsgf(Profile->IsA(USimProfileCamp::StaticClass())||Profile->GetClass()->ImplementsInterface(USimProfileContainer::StaticClass()), TEXT("Invalid profile class [%s] for profile component [%s]"), *Profile->GetClass()->GetName(), *GetClass()->GetName());
	for(auto& elem : StartSquads)
	{
		ISimProfileContainer::Execute_AddItem(Profile, DuplicateObject(elem, GetWorld()));
	}
	return Profile;
}


// Called every frame
void UProfileComponentCamp::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

