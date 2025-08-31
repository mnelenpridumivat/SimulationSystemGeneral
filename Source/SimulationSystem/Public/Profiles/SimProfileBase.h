// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define NoSimEnsureMessage

#include "CoreMinimal.h"
#include "SimProfileID.h"
#include "Containers/Union.h"
#include "Interfaces/SimProfileContainer.h"
#include "SimProfileHolder.h"
#include "SimulationLevels.h"
#include "Engine/StreamableManager.h"
#include "UObject/Object.h"
#include "SimProfileBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SIMULATIONSYSTEM_API USimProfileBase : public UObject
{
	GENERATED_BODY()

public:
	USimProfileBase();

	/*
	 * Called when profile added to global graph (as standalone profile or as child of other profile)
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnRegistered();

	/*
	 * Called when profile loaded from save
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnLoaded();

	/*
	 * Called when profile created for first time
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnCreated();

	/*
	 * Called every tick
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Tick(float DeltaTime);
	
	UFUNCTION(BlueprintNativeEvent)
	bool IsMovable();

	UFUNCTION(BlueprintPure)
	FORCEINLINE FSimProfileID GetProfileID(){return ProfileID;}

	void SetProfileID(const FSimProfileID& NewID)
	{
		ProfileID = NewID;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE TEnumAsByte<ESimulationLevels> GetSimLevel(){return CurrentSimLevel;}

	/*
	 * Switching between simulation levels (online/buffered/offline)
	 */
	void SetSimLevel(ESimulationLevels NewLevel){CurrentSimLevel = NewLevel;}

	/*
	 * Called when profile exit graph vertex or parent profile
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnExit(const FSimProfileHolder& Location);

	/*
	 * Called when profile enters vertex or parent profile (excluding first spawn)
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnEnter(const FSimProfileHolder& Location);

	virtual USimProfileBase* DeepCopyProfile();

	virtual void TransitOfflineBuffered();
	virtual void TransitBufferedOffline();
	virtual void TransitBufferedOnline();
	virtual void TransitOnlineBuffered();
	void TransitOfflineOnline(){TransitOfflineBuffered();TransitBufferedOnline();}
	void TransitOnlineOffline(){TransitOnlineBuffered();TransitBufferedOffline();}
	
	void OnOnlineActorClassLoaded();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasOnlineActor() const {return OnlineActorClass.IsValid();}

	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetOnlineActor() const {return OnlineActor;}

	UFUNCTION(BlueprintCallable)
	virtual void SetOnlineLocation(FVector Vector);

	UFUNCTION(BlueprintPure)
	FORCEINLINE FTransform GetOnlineLocation() const;

	FORCEINLINE void SetOnlineActorClass(const TSoftClassPtr<AActor>& NewClass){OnlineActorClass = NewClass;}

	/*
	 * Using during loading from save, do not use!
	 */
	void SetOnlineActor(AActor* Actor){OnlineActor = Actor;}
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override {return true;}

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AActor> OnlineActorClass;

	UPROPERTY()
	FTransform OnlineLocation;

	UPROPERTY()
	FSimProfileID ProfileID;

	UPROPERTY()
	TEnumAsByte<ESimulationLevels> CurrentSimLevel = ESimulationLevels_Offline;

	UPROPERTY(Transient)
	AActor* OnlineActor = nullptr;

	TSharedPtr<FStreamableHandle> StreamableHandle = nullptr;
};
