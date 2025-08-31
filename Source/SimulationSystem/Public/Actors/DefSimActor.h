// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimActorInterface.h"
#include "GameFramework/Actor.h"
#include "DefSimActor.generated.h"

UCLASS()
class SIMULATIONSYSTEM_API ADefSimActor : public AActor, public ISimActorInterface
{
public:
	virtual bool UseInSimulation_Implementation() override {return true;}
	virtual UProfileComponent* GetProfileComponent_Implementation() override {return ProfileComponent;};

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADefSimActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	TObjectPtr<UProfileComponent> ProfileComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
