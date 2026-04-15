// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionPlanner.h"
#include "MotivatorFormulaVM.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "MotivatorConfigDataAsset.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SimulationSystem.FDriverFunctionNode", ExcludeBaseStruct))
	FInstancedStruct Instance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "Type==EMotivatorClampNormalizationType::Sigmoid"))
	float Steepness = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "Type==EMotivatorClampNormalizationType::Sigmoid"))
	float Center = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMotivatorClampNormalizationType Type = EMotivatorClampNormalizationType::Clamp;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationNegate : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SimulationSystem.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SimulationSystem.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> Instances;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationMul : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SimulationSystem.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> Instances;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationMax : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SimulationSystem.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> Instances;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationMin : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SimulationSystem.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> Instances;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationAverage : public FDriverFunctionNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SimulationSystem.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	TArray<FInstancedStruct> Instances;
};

USTRUCT(BlueprintType)
struct FDriverFunctionOperationAverageWeightedElem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SimulationSystem.FDriverFunctionNode", ExcludeBaseStruct))
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BaseStruct = "/Script/SimulationSystem.FDriverFunctionNode", ExcludeBaseStruct, ShowTreeView))
	FInstancedStruct Instance;
 
	friend FArchive& operator<<(FArchive& Ar, FDriverFunction& Op)
	{
		auto type = StaticStruct();
		type->SerializeItem(Ar, &Op, nullptr);
		return Ar;
	}
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ShowTreeView, EditorOnly))
	TArray<FDriverFunction> ContextModifiers;

	/*
	 * Bytecode version of ContextModifiers, autogenerated on save for runtime
	 */
	UPROPERTY()
	TArray<FMotivatorFormulaVMCode> ContextModifiersBytecode;
	
	bool Serialize(FArchive& Ar);
};

template<>
struct TStructOpsTypeTraits<FMotivatorPossibleResult> : public TStructOpsTypeTraitsBase2<FMotivatorPossibleResult>
{
	enum
	{
		WithSerializer = true,
	};
};

/**
 * 
 */
UCLASS(BlueprintType)
class SIMULATIONSYSTEM_API UMotivatorConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

#if WITH_EDITOR
	void Compile(const FDriverFunction& Config, FMotivatorFormulaVMCode& BytecodeSlot);
	void CompileRec(const FInstancedStruct& Operation, FMotivatorFormulaVMCode& BytecodeSlot);
#endif
	
public:

	static bool FindGoal(UMotivatorConfigDataAsset* AssetCDO, const FActionStorage& CurrentState, FActionPlannerGoal& GoalState);
	
#if WITH_EDITOR
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif

#if WITH_EDITORONLY_DATA
	/*
	 * Take infinite amount of facts as "yes/no" and collapse into finite number of mental traits
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditoOnly, SkipSerialization))
	TMap<FName, FDriverFunction> MentalTraitFunctions;
#endif

	/*
	 * Bytecode version of MentalTraitFunctions, autogenerated on save for runtime
	 */
	UPROPERTY()
	TMap<FName, FMotivatorFormulaVMCode> MentalTraitBytecodes;
	
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
