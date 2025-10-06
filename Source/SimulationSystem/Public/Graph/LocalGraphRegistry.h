// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LocalGraphRegistry.generated.h"

class UAISimProfileSquad;
class USimProfileCamp;
class AGraphAsset;
class USimProfileBase;
/**
 * 
 */
UCLASS(Blueprintable)
class SIMULATIONSYSTEM_API ULocalGraphRegistry : public UObject
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TSet<USimProfileBase*> ProfilesInside;

	UPROPERTY(Transient)
	TSet<USimProfileCamp*> CampsInside;

	UPROPERTY(Transient)
	TSet<UAISimProfileSquad*> SquadsInside;

	UPROPERTY()
	AGraphAsset* ParentGraphAsset = nullptr;

public:

	void SetLocalGraph(AGraphAsset* GraphAsset);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProfileEntered, USimProfileBase*, Profile);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProfileExited, USimProfileBase*, Profile);

	UPROPERTY(BlueprintAssignable)
	FOnProfileEntered OnProfileEntered;

	UPROPERTY(BlueprintAssignable)
	FOnProfileExited OnProfileExited;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSquadEntered, UAISimProfileSquad*, Profile);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSquadExited, UAISimProfileSquad*, Profile);

	UPROPERTY(BlueprintAssignable)
	FOnSquadEntered OnSquadEntered;

	UPROPERTY(BlueprintAssignable)
	FOnSquadExited OnSquadExited;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void RegisterProfile(USimProfileBase* Profile);
public:
	void NativeRegisterProfile(USimProfileBase* Profile);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UnregisterProfile(USimProfileBase* Profile);
public:
	void NativeUnregisterProfile(USimProfileBase* Profile);

	UFUNCTION(BlueprintPure)
	void GetAllProfiles(TArray<USimProfileBase*>& Profiles);

	UFUNCTION(BlueprintPure)
	void GetAllCamps(TArray<USimProfileCamp*>& Camps);

	UFUNCTION(BlueprintPure)
	void GetAllSquads(TArray<UAISimProfileSquad*>& Squads);
	
};
