// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AISimProfileBase.h"
#include "AISimProfilePawn.generated.h"

class USimProfileItem;
/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories, Blueprintable)
class SIMULATIONSYSTEM_API UAISimProfilePawn : public UAISimProfileBase, public ISimProfileContainer
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
	
	virtual void Save_Implementation(FSimVertexID VertexID, FSerializedProfileView Data) override;
	virtual void Load_Implementation(FSerializedProfile& Data) override;

private:
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
	TArray<USimProfileItem*> Inventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category="Characteristics")
	TMap<FName, int> CharacteristicsList;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category="Characteristics")
	TMap<FName, float> PointsList;
	
};
