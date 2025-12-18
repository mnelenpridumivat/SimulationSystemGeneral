// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProfileComponent.h"
#include "ProfileGeneratorSquad.h"
#include "ProfileComponentCamp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMULATIONSYSTEM_API UProfileComponentCamp : public UProfileComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProfileComponentCamp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual USimProfileBase* CreateNewProfile_Implementation() override;

	UPROPERTY(EditAnywhere)
	TArray<FSimulationArchetypeHandle> StartSquads;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void InitProfile(FProfilesSerialized& Return, const FSimVertexID& VertexID) override;
};
