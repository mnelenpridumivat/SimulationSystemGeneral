// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define NoSimEnsureMessage

#include "CoreMinimal.h"
#include "DebugData.h"
#include "SimProfileID.h"
#include "Containers/Union.h"
#include "Interfaces/SimProfileContainer.h"
#include "SimProfileHolder.h"
#include "SimulationLevels.h"
#include "Engine/StreamableManager.h"
#include "UObject/Object.h"
#include "SimProfileBase.generated.h"

struct FSerializedProfileView;
struct FSerializedProfile;
/**
 * 
 */
UCLASS(BlueprintType)
class SIMULATIONSYSTEM_API USimProfileBase : public UObject
{
	GENERATED_BODY()

public:
	USimProfileBase();

protected:
	/*
	 * Called when saving
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void Save(FSimVertexID VertexID, FSerializedProfileView Data);
public:
	virtual void NativeSave(FSimVertexID VertexID, FSerializedProfileView Data);

protected:
	/*
	 * Called when loading
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void Load(FSerializedProfile& Data);
public:
	virtual void NativeLoad(FSerializedProfile& Data);

protected:
	/*
	 * Called when profile added to global graph (as standalone profile or as child of other profile)
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnRegistered();
public:
	virtual void NativeOnRegistered();

protected:
	/*
	 * Called after all profiles from save added to graph
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnPostRegistered();
public:
	virtual void NativeOnPostRegistered();

protected:
	/*
	 * Called when profile loaded from save
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnLoaded();
public:
	virtual void NativeOnLoaded();

protected:
	/*
	 * Called when profile created for first time
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnCreated();
public:
	virtual void NativeOnCreated();


protected:
	/*
	 * Called every tick
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void Tick(float DeltaTime);
public:
	virtual void NativeTick(float DeltaTime);
	
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
	FTransform GetOnlineLocation() const;

	FORCEINLINE void SetOnlineActorClass(const TSoftClassPtr<AActor>& NewClass){OnlineActorClass = NewClass;}

	/*
	 * Using during loading from save, do not use!
	 */
	void SetOnlineActor(AActor* Actor){OnlineActor = Actor;}
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override {return true;}

	UFUNCTION(BlueprintPure)
	float GetTickOfflineCoeff(){return TickOfflineCoeff;}

	UFUNCTION(BlueprintPure)
	float GetTickBufferedCoeff(){return TickBufferedCoeff;}

	UFUNCTION(BlueprintPure)
	float GetTickOnlineCoeff(){return TickOnlineCoeff;}

	void GetDebugData(FDebugDataMain& Data);

private:
	void AddPropFunc(DebugDataPropertyHandle Prop, TArray<DebugDataElemBase*>& Arr);
	void AddArrayPropFunc(DebugDataPropertyHandle Property, TArray<DebugDataElemBase*>& Arr);
	void AddSetPropFunc(DebugDataPropertyHandle Property, TArray<DebugDataElemBase*>& Arr);
	void AddMapPropFunc(DebugDataPropertyHandle Property, TArray<DebugDataElemBase*>& Arr);
	void AddSinglePropFunc(DebugDataPropertyHandle Property, TArray<DebugDataElemBase*>& Arr);
	void AddStructPropFunc(DebugDataPropertyHandle Property, TArray<DebugDataElemBase*>& Arr);
	
public:

	float TimeSinceLastTick = 0.0f;

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

	// coeffs of delta between ticks for each sim state (less number - more often updates)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TickOfflineCoeff = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TickBufferedCoeff = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TickOnlineCoeff = 1.0f;
	
};
