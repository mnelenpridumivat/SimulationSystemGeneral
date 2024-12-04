// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationLevels.h"
#include "UObject/Object.h"
#include "SquadTaskBase.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USquadTaskBase : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	bool Aborted = false;

public:

	FORCEINLINE bool IsAborted(){return Aborted;}

	void BeginTask(UAISimProfileSquad* Squad);
	void ExecuteTask(UAISimProfileSquad* Squad, float DeltaTime);
	void FinishTask(UAISimProfileSquad* Squad, bool Success);
	bool CheckTaskFinished(UAISimProfileSquad* Squad);

	UFUNCTION(BlueprintNativeEvent)
	void AbortTask(UAISimProfileSquad* Squad);

	UFUNCTION(BlueprintNativeEvent)
	void BeginTask_Always(UAISimProfileSquad* Squad);

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteTask_Always(UAISimProfileSquad* Squad, float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void FinishTask_Always(UAISimProfileSquad* Squad, bool Success);

	UFUNCTION(BlueprintNativeEvent)
	bool CheckTaskFinished_Always(UAISimProfileSquad* Squad);

	UFUNCTION(BlueprintNativeEvent)
	void BeginTask_Online(UAISimProfileSquad* Squad);

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteTask_Online(UAISimProfileSquad* Squad, float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void FinishTask_Online(UAISimProfileSquad* Squad, bool Success);

	UFUNCTION(BlueprintNativeEvent)
	bool CheckTaskFinished_Online(UAISimProfileSquad* Squad);

	UFUNCTION(BlueprintNativeEvent)
	void BeginTask_Offline(UAISimProfileSquad* Squad);

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteTask_Offline(UAISimProfileSquad* Squad, float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void FinishTask_Offline(UAISimProfileSquad* Squad, bool Success);

	UFUNCTION(BlueprintNativeEvent)
	bool CheckTaskFinished_Offline(UAISimProfileSquad* Squad);

	UFUNCTION(BlueprintNativeEvent)
	void BeginTask_Buffer(UAISimProfileSquad* Squad);

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteTask_Buffer(UAISimProfileSquad* Squad, float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void FinishTask_Buffer(UAISimProfileSquad* Squad, bool Success);

	UFUNCTION(BlueprintNativeEvent)
	bool CheckTaskFinished_Buffer(UAISimProfileSquad* Squad);
	
};
