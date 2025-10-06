// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimProfileBase.h"
#include "SimProfileCamp.generated.h"

class UAISimProfileSquad;
/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories, Blueprintable)
class SIMULATIONSYSTEM_API USimProfileCamp : public USimProfileBase, public ISimProfileContainer
{
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSquadEntered, USimProfileCamp*, ToCamp, UAISimProfileSquad*, Squad);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSquadExited, USimProfileCamp*, FromCamp, UAISimProfileSquad*, Squad);

	UPROPERTY(BlueprintAssignable)
	FOnSquadEntered OnSquadEntered;

	UPROPERTY(BlueprintAssignable)
	FOnSquadExited OnSquadExited;
	
	virtual USimProfileBase* DeepCopyProfile() override;

	virtual void AddItem_Implementation(USimProfileBase* Profile) override;
	virtual TArray<USimProfileBase*> GetAllItems_Implementation() override;
	virtual bool HasItem_Implementation(USimProfileBase* Profile) override;
	virtual void RemoveItem_Implementation(USimProfileBase* Profile) override;
	virtual bool CanStoreItem_Implementation(USimProfileBase* Profile) override;

private:
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
	TArray<UAISimProfileSquad*> Squads;

public:
	virtual bool IsMovable_Implementation() override;
	virtual void NativeOnRegistered() override;
	virtual void NativeSave(FSimVertexID VertexID, FSerializedProfileView Data) override;
	virtual void NativeLoad(FSerializedProfile& Data) override;
};
