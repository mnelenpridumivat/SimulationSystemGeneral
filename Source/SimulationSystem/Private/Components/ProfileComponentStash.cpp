// Fill out your copyright notice in the Description page of Project Settings.


#include "ProfileComponentStash.h"

#include "SimProfileBase.h"
#include "SimProfileContainer.h"
#include "SimProfileStash.h"


// Sets default values for this component's properties
UProfileComponentStash::UProfileComponentStash()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProfileComponentStash::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

USimProfileBase* UProfileComponentStash::CreateNewProfile_Implementation()
{
	auto Profile = Super::CreateNewProfile_Implementation();
	ensureMsgf(Profile->IsA(USimProfileStash::StaticClass())||Profile->GetClass()->ImplementsInterface(USimProfileContainer::StaticClass()), TEXT("Invalid profile class [%s] for profile component [%s]"), *Profile->GetClass()->GetName(), *GetClass()->GetName());
	for(auto& elem : StartContent)
	{
		ISimProfileContainer::Execute_AddItem(Profile, DuplicateObject(elem, GetWorld()));
	}
	return Profile;
}


// Called every frame
void UProfileComponentStash::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

