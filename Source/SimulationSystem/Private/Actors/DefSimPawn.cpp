// Fill out your copyright notice in the Description page of Project Settings.


#include "DefSimPawn.h"

#include "ProfileComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ADefSimPawn::ADefSimPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProfileComponent = CreateDefaultSubobject<UProfileComponent>("ProfileComponent");
}

// Called when the game starts or when spawned
void ADefSimPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefSimPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADefSimPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADefSimPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADefSimPawn, ProfileComponent);
}

