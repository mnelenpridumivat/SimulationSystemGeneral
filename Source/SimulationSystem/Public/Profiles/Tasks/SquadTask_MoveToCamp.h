// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphWay.h"
#include "SimVertexID.h"
#include "SquadTaskBase.h"
#include "SquadTask_MoveToCamp.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USquadTask_MoveToCamp : public USquadTaskBase
{
public:

	virtual void Serialize(FArchive& Ar) override;

private:
	GENERATED_BODY()

public:
	virtual void BeginTask_Offline_Implementation(UAISimProfileSquad* Squad) override;
	virtual void ExecuteTask_Offline_Implementation(UAISimProfileSquad* Squad, float DeltaTime) override;
	virtual void FinishTask_Offline_Implementation(UAISimProfileSquad* Squad, bool Success) override;
	virtual bool CheckTaskFinished_Offline_Implementation(UAISimProfileSquad* Squad) override;
	virtual void BeginTask_Buffer_Implementation(UAISimProfileSquad* Squad) override;
	virtual void ExecuteTask_Buffer_Implementation(UAISimProfileSquad* Squad, float DeltaTime) override;
	virtual void FinishTask_Buffer_Implementation(UAISimProfileSquad* Squad, bool Success) override;
	virtual bool CheckTaskFinished_Buffer_Implementation(UAISimProfileSquad* Squad) override;

	void SetupTask(const FSimVertexID& Start, const FSimVertexID& Dest);

	FORCEINLINE const FGraphWay& GetWay() const {return Way;}
	FORCEINLINE float GetLocalDistance() const {return LocalDistance;}
	FORCEINLINE float GetLocalDistanceCompleted() const {return LocalDistanceCompleted;}

private:

	//UPROPERTY()
	FGraphWay Way;

	UPROPERTY()
	float LocalDistance = 0;

	UPROPERTY()
	float LocalDistanceCompleted = 0;
	
};
