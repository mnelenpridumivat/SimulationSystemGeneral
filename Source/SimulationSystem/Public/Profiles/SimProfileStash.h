// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimProfileBase.h"
#include "SimProfileStash.generated.h"

class USimProfileItem;
/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories, Blueprintable)
class SIMULATIONSYSTEM_API USimProfileStash : public USimProfileBase, public ISimProfileContainer
{
public:
	virtual USimProfileBase* DeepCopyProfile() override;
	
	virtual void OnRegistered_Implementation() override;
	virtual bool IsMovable_Implementation() override;

	virtual void AddItem_Implementation(USimProfileBase* Profile) override;
	virtual TArray<USimProfileBase*> GetAllItems_Implementation() override;
	virtual bool HasItem_Implementation(USimProfileBase* Profile) override;
	virtual void RemoveItem_Implementation(USimProfileBase* Profile) override;
	virtual bool CanStoreItem_Implementation(USimProfileBase* Profile) override;
	
	virtual void Save_Implementation(FSimVertexID VertexID, FSerializedProfileView Data) override;
	virtual void Load_Implementation(FSerializedProfile& Data) override;

protected:

	UPROPERTY(Transient)
	TArray<USimProfileItem*> StoredItems;

private:
	GENERATED_BODY()
};
