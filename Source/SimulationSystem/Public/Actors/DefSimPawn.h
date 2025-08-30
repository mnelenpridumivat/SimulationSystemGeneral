// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimActorInterface.h"
#include "GameFramework/Pawn.h"
#include "DefSimPawn.generated.h"

UCLASS()
class SIMULATIONSYSTEM_API ADefSimPawn : public APawn, public ISimActorInterface
{
public:
	virtual UProfileComponent* GetProfileComponent_Implementation() override {return ProfileComponent;};

private:
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADefSimPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	TObjectPtr<UProfileComponent> ProfileComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
