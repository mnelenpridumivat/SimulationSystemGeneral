// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommandBuffer.h"
#include "UObject/Object.h"
#include "Action.generated.h"

class UActionPlannerOwner;
struct FActionPlannerGoal;
struct FActionStorage;
class UActionPlanner;
class UActionEffect;
class UActionPrecondition;

UENUM(BlueprintType)
enum class EActionStatus : uint8
{
	Idle,
	Running,
	Aborted,
	Finished,
};

/**
 * 
 */
UCLASS(Blueprintable)
class SIMULATIONSYSTEM_API UAction : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UWorld> World;

	UPROPERTY()
	TObjectPtr<UActionPlanner> ParentPlanner;

	UPROPERTY()
	TArray<TObjectPtr<UActionPrecondition>> PreconditionsObjs;

	UPROPERTY()
	TArray<TObjectPtr<UActionEffect>> EffectsObjs;

	UPROPERTY()
	EActionStatus Status;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, bool> Preconditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, bool> Effects;

public:

	void SetParentPlanner(UActionPlanner* Planner);
	void BeginPlay();

	bool CheckPreconditions(const FActionStorage& Storage);
	bool CheckSelfPreconditions();

	bool CheckIfWeCanCreateState(const FActionPlannerGoal& Storage);
	void GetRequiredStartGoal(FActionPlannerGoal& Storage);

protected:

	UFUNCTION(BlueprintPure)
	UActionPlannerOwner* GetOwningObject() const;

	UFUNCTION(BlueprintCallable)
	void FinishAction();
	
public:
	
	UFUNCTION(BlueprintCallable)
	void AbortAction();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void StartAction();
public:
	void NativeStartAction();
	virtual void NativeStartAction(FMassCommandBuffer& CmdBuffer){}
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void ProcessAction();
public:
	EActionStatus NativeProcessAction();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void EndAction(bool Successful);
public:
	void NativeEndAction(bool Successful);

	virtual UWorld* GetWorld() const override;
	virtual void PostInitProperties() override;
	
};
