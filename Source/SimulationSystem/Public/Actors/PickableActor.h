// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProfileComponent.h"
#include "SimActorInterface.h"
#include "GameFramework/Actor.h"
#include "PickableActor.generated.h"

class UProfileGenerator;

UCLASS()
class SIMULATIONSYSTEM_API APickableActor : public AActor, public ISimActorInterface
{
public:
	virtual UProfileComponent* GetProfileComponent_Implementation() override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProfileComponent* ProfileComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
