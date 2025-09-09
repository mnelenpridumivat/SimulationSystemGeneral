// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimProfileID.h"
#include "Components/ActorComponent.h"
#include "ProfileComponent.generated.h"


class ADEPRECATED_ReplicatedSimInfo;
class UProfileGenerator;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMULATIONSYSTEM_API UProfileComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_DELEGATE_RetVal(USimProfileBase*, FConstructProfile);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Sets default values for this component's properties
	UProfileComponent();

	//UPROPERTY(BlueprintAssignable)
	//FConstructProfile ConstructProfile;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintNativeEvent)
	USimProfileBase* CreateNewProfile();

	UPROPERTY(EditAnywhere, Instanced)
	USimProfileBase* ProfileBase;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated)
	FSimProfileID ProfileID;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TObjectPtr<USimProfileBase> Profile;

	//UPROPERTY(BlueprintReadOnly, Replicated)
	//AReplicatedSimInfo* ReplicatedInfo;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Called on online actor spawn
	void SetProfileID(const FSimProfileID& NewProfileID);
	
	// Called on editor compile
	USimProfileBase* InitProfile();

	// Server-only getter of profile of online actor
	UFUNCTION(BlueprintPure)
	USimProfileBase* GetProfile();

	// Getter of replicated profile info, exists both on server and clients
	//UFUNCTION(BlueprintPure)
	//AReplicatedSimInfo* GetReplicatedInfo();
};
