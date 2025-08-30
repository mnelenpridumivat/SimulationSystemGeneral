// Fill out your copyright notice in the Description page of Project Settings.


#include "DefSimCharacter.h"

#include "DefSimActor.h"
#include "ProfileComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ADefSimCharacter::ADefSimCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProfileComponent = CreateDefaultSubobject<UProfileComponent>("ProfileComponent");
}

// Called when the game starts or when spawned
void ADefSimCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefSimCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADefSimCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADefSimCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADefSimCharacter, ProfileComponent);
}

