// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProfileComponent.h"
#include "GameFramework/Info.h"
#include "DEPRECATED_ReplicatedSimInfo.generated.h"

/**
 * A class for data, which needs to be replicated while in online. It doesn't replicate if profile is offline.
 * This class created in order to not replicate all profiles to client and restrict clients from server-side info. 
 */
UCLASS(Deprecated)
class SIMULATIONSYSTEM_API ADEPRECATED_ReplicatedSimInfo : public AInfo
{
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	void GetChildren(TArray<FSimProfileID>& ChildrenArray) const;

	// Call only on server side!
	void SetProfileID(FSimProfileID NewProfileID);

	UFUNCTION(BlueprintPure)
	FSimProfileID GetProfileID() const;
	
	void AddChild(USimProfileBase* Profile);
	void RemoveChild(USimProfileBase* Profile);
	//virtual void BeginDestroy() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	GENERATED_BODY()

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetProfileID(FReplicatedSimProfileID ReplicatedProfileID);

protected:

	UPROPERTY()
	FSimProfileID ProfileID;

	// TODO: Refactor array replication with something faster
	UPROPERTY(Replicated)
	TArray<FSimProfileID> ReplicatedInfoChildren;
	
};
