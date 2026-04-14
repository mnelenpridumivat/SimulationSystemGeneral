// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "MotivatorConfigDataAsset.generated.h"

// TODO: This is very convenient in editor time, but not in runtime.
// The best - make some kind of bytecode compiler with mini VM
// For tests - may be simple tree of operators will be enough

UENUM(BlueprintType)
enum class EMotivatorClampNormalizationType : uint8
{
	Clamp, // Just clamp value in borders
	Sigmoid, // Use sigmoid function
};

USTRUCT(BlueprintType)
struct FDriverFunctionNode
{
	GENERATED_BODY()
};

/*
 * A leaf in tree structure, which represents convertion of binary fact into float weight
 */
USTRUCT(BlueprintType)
struct FDriverFunctionParam : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Knowledge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoeffHas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoeffDontHas;
	
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationClamp : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SIMULATIONSYSTEM.FDriverFunctionNode", ExcludeBaseStruct))
	FInstancedStruct Instance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMotivatorClampNormalizationType Type = EMotivatorClampNormalizationType::Clamp;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationNegate : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SIMULATIONSYSTEM.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	FInstancedStruct Instance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NegatePivot = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ValueCoeff = 1.0f;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationSum : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SIMULATIONSYSTEM.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> Instances;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationMul : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SIMULATIONSYSTEM.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> Instances;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationMax : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SIMULATIONSYSTEM.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> Instances;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationMin : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SIMULATIONSYSTEM.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> Instances;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationAverage : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SIMULATIONSYSTEM.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> Instances;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationAverageWeightedElem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SIMULATIONSYSTEM.FDriverFunctionNode", ExcludeBaseStruct))
	FInstancedStruct Instance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;
	
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationAverageWeighted : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ShowTreeView))
	TArray<FDriverFunctionOperationAverageWeightedElem> Instances;
};



USTRUCT(BlueprintType)
struct FDriverFunction
{
	GENERATED_BODY()

	/*
	 * Place to build formula for mental trait calculation from binary facts
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SIMULATIONSYSTEM.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	FInstancedStruct Instance;
};

USTRUCT(BlueprintType)
struct FMotivatorPossibleResult
{
	GENERATED_BODY()

	/*
	 * Binary data, which describes desired state
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, bool> SetState;

	/*
	 * weight for formula "Raw utility = sum(each mental trait value x weight)"
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> WeightPerMentalTrait;

	/*
	 * Context modifiers from raw knowledge for formula "Final utility = Raw utility + sum(each context modifier)"
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SIMULATIONSYSTEM.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> ContextModifiers;
};

/**
 * 
 */
UCLASS(BlueprintType)
class SIMULATIONSYSTEM_API UMotivatorConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	/*
	 * Take infinite amount of facts as "yes/no" and collapse into finite number of mental traits
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FDriverFunction> MentalTraitFunctions;

	/*
	 * Calculate utility score for each desired state and take the biggest
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMotivatorPossibleResult> Decisions;

	/*
	 * Inertia of changing mental traits from previous value;
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0, UIMax = 1.0))
	float InertiaFactor = 0.0f;

	/*
	 * Some random deviation from exact utility value - to add random in action
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = 0.0f, UIMin = 0.0f, EditCondition = bEnableDecisionNoise))
	float DecisionNoiseRange = 0.0f;
};
