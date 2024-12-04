// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProfileComponent.h"
#include "ProfileComponentStash.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMULATIONSYSTEM_API UProfileComponentStash : public UProfileComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProfileComponentStash();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual USimProfileBase* CreateNewProfile_Implementation() override;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<USimProfileBase*> StartContent;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
