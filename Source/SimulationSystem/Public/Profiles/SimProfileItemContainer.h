// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimProfileItem.h"
#include "SimProfileItemContainer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SIMULATIONSYSTEM_API USimProfileItemContainer : public USimProfileItem, public ISimProfileContainer
{
public:
	virtual USimProfileBase* DeepCopyProfile() override;
	
	virtual void AddItem_Implementation(USimProfileBase* Profile) override;
	virtual TArray<USimProfileBase*> GetAllItems_Implementation() override;
	virtual bool HasItem_Implementation(USimProfileBase* Profile) override;
	virtual void RemoveItem_Implementation(USimProfileBase* Profile) override;
	virtual bool CanStoreItem_Implementation(USimProfileBase* Profile) override;

	virtual void OnRegistered_Implementation() override;
	virtual void SetOnlineLocation(FVector Vector) override;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<USimProfileItem*> StoredItems;

private:
	GENERATED_BODY()
	
};
