// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ConcentricalCircle.generated.h"

class USimProfileBase;
class UAISimProfileSquad;
/**
 * Class for fast search of suitable squad based on distance
 * All arrays needs to update manually by calling update functions
 * Distance calculates using positions of CentralObject graph vertex and target squad graph vertex
 */
UCLASS()
class SIMULATIONSYSTEM_API UConcentricalCircle : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<USimProfileBase> CenterObject;

	/*
	 * Squads on distance <= VeryCloseRadius
	 */
	UPROPERTY()
	TSet<UAISimProfileSquad*> VeryCloseSquads;

	/*
	 * Squads on distance: VeryCloseRadius < S <= CloseRadius
	 */
	UPROPERTY()
	TSet<UAISimProfileSquad*> CloseSquads;

	/*
	 * Squads on distance: CloseRadius < S <= FarRadius
	 */
	UPROPERTY()
	TSet<UAISimProfileSquad*> FarSquads;

	/*
	 * Squads on distance > FarRadius
	 */
	UPROPERTY()
	TSet<UAISimProfileSquad*> VeryFarSquads;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bOverrideVeryCloseDetection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "!bOverrideVeryCloseDetection"))
	float VeryCloseRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bOverrideCloseDetection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "!bOverrideCloseDetection"))
	float CloseRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideFarDetection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "!bOverrideFarDetection"))
	float FarRadius;

public:

	UFUNCTION(BlueprintCallable)
	void SetCenterObject(TObjectPtr<USimProfileBase> SimProfile);

	DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FDistanceVerificationOverride, USimProfileBase*, Center, UAISimProfileSquad*, Target);

	UPROPERTY(BlueprintAssignable)
	FDistanceVerificationOverride VeryCloseDistanceVerification;

	UPROPERTY(BlueprintAssignable)
	FDistanceVerificationOverride CloseDistanceVerification;

	UPROPERTY(BlueprintAssignable)
	FDistanceVerificationOverride FarDistanceVerification;

	UFUNCTION(BlueprintCallable)
	void RegisterSquad(UAISimProfileSquad* NewSquad);

	UFUNCTION(BlueprintCallable)
	void UnregisterSquad(UAISimProfileSquad* RemovedSquad);

	UFUNCTION(BlueprintCallable)
	void UpdateSquad(UAISimProfileSquad* UpdatedSquad);

	UFUNCTION(BlueprintPure)
	void GetVeryCloseSquads(TArray<UAISimProfileSquad*>& Squads);

	UFUNCTION(BlueprintPure)
	void GetCloseSquads(TArray<UAISimProfileSquad*>& Squads);

	UFUNCTION(BlueprintPure)
	void GetFarSquads(TArray<UAISimProfileSquad*>& Squads);

	UFUNCTION(BlueprintPure)
	void GetVeryFarSquads(TArray<UAISimProfileSquad*>& Squads);

	UFUNCTION(BlueprintPure)
	float GetVeryCloseRadius() {return VeryCloseRadius;}

	UFUNCTION(BlueprintPure)
	float GetCloseRadius() {return CloseRadius;}

	UFUNCTION(BlueprintPure)
	float GetFarRadius() {return FarRadius;}

private:
	
	bool IsSuitableFor(UAISimProfileSquad* Squad, bool UseOverride, float DefDistance, FDistanceVerificationOverride& Override) const;
};
