// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AISimProfileBase.h"
#include "AISimProfileSquad.generated.h"

struct FSerializedProfileView;
class USquadTaskBase;
class UAISimProfilePawn;
/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories, Blueprintable)
class SIMULATIONSYSTEM_API UAISimProfileSquad : public UAISimProfileBase, public ISimProfileContainer
{
public:
	virtual void NativeOnCreated() override;
	virtual void NativeOnLoaded() override;

	virtual USimProfileBase* DeepCopyProfile() override;
	
	virtual void AddItem_Implementation(USimProfileBase* Profile) override;
	virtual TArray<USimProfileBase*> GetAllItems_Implementation() override;
	virtual bool HasItem_Implementation(USimProfileBase* Profile) override;
	virtual void RemoveItem_Implementation(USimProfileBase* Profile) override;
	virtual bool CanStoreItem_Implementation(USimProfileBase* Profile) override;

	virtual void NativeOnRegistered() override;
	virtual void SetOnlineLocation(FVector Vector) override;
	
	virtual void NativeSave(FSimVertexID VertexID, FSerializedProfileView Data) override;
	virtual void NativeLoad(FSerializedProfile& Data) override;

private:
	GENERATED_BODY()

	void RefreshSquadCharacteristics();

protected:

	UPROPERTY(Transient)
	TArray<UAISimProfilePawn*> Members;

	UPROPERTY()
	USquadTaskBase* CurrentTask;

public:

	UFUNCTION(BlueprintPure)
	FORCEINLINE USquadTaskBase* GetCurrentTask(){ return CurrentTask; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentTask(USquadTaskBase* NewCurrentTask){ CurrentTask = NewCurrentTask; }

	UFUNCTION(BlueprintPure)
	float GetOfflineSpeed();
	
	virtual void NativeTick(float DeltaTime) override;
};
