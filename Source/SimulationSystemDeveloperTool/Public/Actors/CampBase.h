// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimProfileID.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SimActorInterface.h"
#include "CampBase.generated.h"

class UProfileComponentCamp;

UCLASS()
class SIMULATIONSYSTEMDEVELOPERTOOL_API ACampBase : public AActor, public ISimActorInterface
{
public:
	virtual UProfileComponent* GetProfileComponent_Implementation() override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACampBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UProfileComponentCamp* ProfileComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
