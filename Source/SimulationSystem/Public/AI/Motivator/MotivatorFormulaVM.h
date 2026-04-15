#pragma once

#include "CoreMinimal.h"
#include "MotivatorFormulaVM.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMotivatorVMState, Display, All);

UENUM()
enum EMotivatorFormulaVMOpcodeType : uint8
{
	/*
	 * Invalid opcode, if this opcode exists in bytecode, compilation failed
	 */
	Invalid = 0,
	
	/*
	 * Convert FName param into float based on transform map and input infos
	 * In: Param name (const, FName), Param index (const, uint16)
	 * Out: Param value (float)
	 */
	PushParam,

	/*
	 * Clamp data between two floats
	 * In: Value (stack, float), Min (const, float), Max (const, float)
	 * Out: Clamped value (float)
	 */
	ClampDefault,

	/*
	 * Clamp data between two floats using Sigmoid function
	 * In: Value (stack, float), Min (const, float), Max (const, float), Steepness (const, float), Center (const, float)
	 * Out: Clamped value (float)
	 */
	ClampSigmoid,

	/*
	 * Negate data around some pivot using Y = A - X*B
	 * In: X (stack, float), A (const, float), B (const, float)
	 * Out: Y (float)
	 */
	NegateSimple, // optimized version then A = 0 and B = 1 (no this args on const stack)
	NegateDefault, // common case

	/*
	 * Take N arguments from stack and process some action, then put result on stack
	 * In: Arr (stack, array of float), N (const, uint16)
	 * Out: Sum (float)
	 */
	Sum, // Sum
	Mul, // Multiplication
	Min, // Minimal value
	Max, // Maximum value
	Avg, // Average value

	/*
	 * Take N arguments from stack and constants and find weighted average value, then return this value (weighted) on stack
	 * In: Arr (stack, array of float), N (const, float), WeightsArr (const, array of float)
	 * Out: Weighted Avg (float)
	 */
	AvgW,
	
};

USTRUCT()
struct FMotivatorFormulaVMOpcode
{
	GENERATED_BODY()
	
	TVariant<EMotivatorFormulaVMOpcodeType, float, FName, uint16> Opcode;
	
	bool Serialize(FArchive& Ar);
 
	friend FArchive& operator<<(FArchive& Ar, FMotivatorFormulaVMOpcode& Op)
	{
		Op.Serialize(Ar);
		return Ar;
	}
};

template<>
struct TStructOpsTypeTraits<FMotivatorFormulaVMOpcode> : public TStructOpsTypeTraitsBase2<FMotivatorFormulaVMOpcode>
{
	enum
	{
		WithSerializer = true,
	};
};

USTRUCT()
struct FMotivatorFormulaVMConvertion
{
	GENERATED_BODY()

	UPROPERTY()
	float ValueHas;

	UPROPERTY()
	float ValueDontHas;
	
	friend FArchive& operator<<(FArchive& Ar, FMotivatorFormulaVMConvertion& Var)
	{
		Ar << Var.ValueHas << Var.ValueDontHas;
		return Ar;
	}
	
};

USTRUCT()
struct FMotivatorFormultVMConvertionVariants
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FMotivatorFormulaVMConvertion> Variants;
	
	friend FArchive& operator<<(FArchive& Ar, FMotivatorFormultVMConvertionVariants& Var)
	{
		Ar << Var.Variants;
		return Ar;
	}
};

USTRUCT()
struct FMotivatorFormulaVMCode
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FMotivatorFormulaVMOpcode> Opcodes;

	UPROPERTY()
	TMap<FName, FMotivatorFormultVMConvertionVariants> Conversions;

	UPROPERTY()
	bool IsValid = false;

	friend FArchive& operator<<(FArchive& Ar, FMotivatorFormulaVMCode& Code)
	{
		Ar << Code.Opcodes;
		Ar << Code.Conversions;
		Ar << Code.IsValid;
		return Ar;
	}
};

struct FMotivatorFormulaVMExecutionContext
{	
	TArray<float> Stack;
	FMotivatorFormulaVMCode* Code = nullptr;
	const TSet<FName>* CurrentKnowledge = nullptr;
	size_t CurrentCodeIndex = 0;

	FMotivatorFormulaVMExecutionContext(FMotivatorFormulaVMCode& NewCode, const TSet<FName>& NewKnowledge);
	
	float Execute();
	void DumpVM();
};