// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimAIController.generated.h"

class UAISimProfileBase;
/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimAIController : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<UAISimProfileBase*, float> Profiles;

	UPROPERTY()
	TSet<UAISimProfileBase*> Players;
	
public:

	void AddProfile(UAISimProfileBase* NewProfile);
	void RemoveProfile(UAISimProfileBase* RemovedProfile);

	void Tick(float DeltaTime);

	void TickOnline(UAISimProfileBase* Profile, float DeltaTime);
	void TickBuffered(UAISimProfileBase* Profile, float DeltaTime);
	void TickOffline(UAISimProfileBase* Profile, float DeltaTime);
	
};
