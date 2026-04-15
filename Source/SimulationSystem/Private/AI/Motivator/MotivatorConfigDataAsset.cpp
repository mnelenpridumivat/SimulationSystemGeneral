// Fill out your copyright notice in the Description page of Project Settings.


#include "MotivatorConfigDataAsset.h"

#include "TPriorityQueue.h"
#include "EntitySystem/MovieSceneEntitySystemTypes.h"
#include "UObject/ObjectSaveContext.h"

bool FMotivatorPossibleResult::Serialize(FArchive& Ar)
{
	Ar << SetState << WeightPerMentalTrait << ContextModifiersBytecode;

	if (!Ar.IsCooking())
	{
		Ar << ContextModifiers;
	}

	return true;
}

#if WITH_EDITOR

void UMotivatorConfigDataAsset::Compile(const FDriverFunction& Config, FMotivatorFormulaVMCode& BytecodeSlot)
{
	CompileRec(Config.Instance, BytecodeSlot);
	BytecodeSlot.IsValid = true;
	for (auto& Opcode : BytecodeSlot.Opcodes)
	{
		if (Opcode.Opcode.IsType<EMotivatorFormulaVMOpcodeType>())
		{
			BytecodeSlot.IsValid = BytecodeSlot.IsValid && Opcode.Opcode.Get<EMotivatorFormulaVMOpcodeType>() != EMotivatorFormulaVMOpcodeType::Invalid;
		}
	}
}

void UMotivatorConfigDataAsset::CompileRec(const FInstancedStruct& Operation, FMotivatorFormulaVMCode& BytecodeSlot)
{
	static auto PushOpcode = [](TArray<FMotivatorFormulaVMOpcode>& Codes, EMotivatorFormulaVMOpcodeType type)
	{
		Codes.AddZeroed();
		Codes.Last().Opcode.Set<EMotivatorFormulaVMOpcodeType>(type);
	};
	static auto PushFloat = [](TArray<FMotivatorFormulaVMOpcode>& Codes, float value)
	{
		Codes.AddZeroed();
		Codes.Last().Opcode.Set<float>(value);
	};
	static auto PushName = [](TArray<FMotivatorFormulaVMOpcode>& Codes, FName value)
	{
		Codes.AddZeroed();
		Codes.Last().Opcode.Set<FName>(value);
	};
	static auto PushWord = [](TArray<FMotivatorFormulaVMOpcode>& Codes, uint16 value)
	{
		Codes.AddZeroed();
		Codes.Last().Opcode.Set<uint16>(value);
	};
	static TMap<UScriptStruct*, TFunction<void(UMotivatorConfigDataAsset*, const FInstancedStruct&, FMotivatorFormulaVMCode&)>> Processors = {
		{
			FDriverFunctionParam::StaticStruct(),
			[](UMotivatorConfigDataAsset* self, const FInstancedStruct& Struct, FMotivatorFormulaVMCode& Slot)
			{
				auto& Op = Struct.Get<FDriverFunctionParam>();
				uint16 KnowledgeSlotIndex = 0;
				auto Conv = Slot.Conversions.Find(Op.Knowledge);
				if (Conv)
				{
					auto Num = Conv->Variants.Num();
					KnowledgeSlotIndex = Num;
					checkf(KnowledgeSlotIndex == Num, TEXT("Knowledge slot overflow!"))
					Conv->Variants.Add({Op.CoeffHas, Op.CoeffDontHas});
				} else
				{
					auto& NewSlot = Slot.Conversions.Add(Op.Knowledge);
					NewSlot.Variants.Add({Op.CoeffHas, Op.CoeffDontHas});
				}
				PushWord(Slot.Opcodes, KnowledgeSlotIndex);
				PushName(Slot.Opcodes, Op.Knowledge);
				PushOpcode(Slot.Opcodes, PushParam);
			}
		},
		{
			FDriverFunctionOperationClamp::StaticStruct(),
			[](UMotivatorConfigDataAsset* self, const FInstancedStruct& Struct, FMotivatorFormulaVMCode& Slot)
			{
				auto& Op = Struct.Get<FDriverFunctionOperationClamp>();
				
				switch (Op.Type)
				{
				case EMotivatorClampNormalizationType::Clamp:
					{
						PushFloat(Slot.Opcodes, Op.Max);
						PushFloat(Slot.Opcodes, Op.Min);
						PushOpcode(Slot.Opcodes, ClampDefault);
						break;
					}
				case EMotivatorClampNormalizationType::Sigmoid:
					{
						PushFloat(Slot.Opcodes, Op.Center);
						PushFloat(Slot.Opcodes, Op.Steepness);
						PushFloat(Slot.Opcodes, Op.Max);
						PushFloat(Slot.Opcodes, Op.Min);
						PushOpcode(Slot.Opcodes, ClampSigmoid);
						break;
					}
				default:
					{
						checkf(false, TEXT("Unknown clamp type [%s]!"), *StaticEnum<EMotivatorClampNormalizationType>()->GetNameStringByValue((int64)Op.Type));
					}
				}

				self->CompileRec(Op.Instance, Slot);
			}
		},
		{
			FDriverFunctionOperationNegate::StaticStruct(),
			[](UMotivatorConfigDataAsset* self, const FInstancedStruct& Struct, FMotivatorFormulaVMCode& Slot)
			{
				auto& Op = Struct.Get<FDriverFunctionOperationNegate>();
				if (FMath::IsNearlyZero(Op.NegatePivot) && FMath::IsNearlyEqual(Op.ValueCoeff, 1.0f))
				{
					PushOpcode(Slot.Opcodes, NegateSimple);
				} else
				{
					PushFloat(Slot.Opcodes, Op.ValueCoeff);
					PushFloat(Slot.Opcodes, Op.NegatePivot);
					PushOpcode(Slot.Opcodes, NegateDefault);
				}
				self->CompileRec(Op.Instance, Slot);
			}
		},
		{
			FDriverFunctionOperationSum::StaticStruct(),
			[](UMotivatorConfigDataAsset* self, const FInstancedStruct& Struct, FMotivatorFormulaVMCode& Slot)
			{
				auto& Op = Struct.Get<FDriverFunctionOperationSum>();
				PushWord(Slot.Opcodes, Op.Instances.Num());
				PushOpcode(Slot.Opcodes, Sum);
				for (auto& elem : Op.Instances)
				{
					self->CompileRec(elem, Slot);
				}
			}
		},
		{
			FDriverFunctionOperationMul::StaticStruct(),
			[](UMotivatorConfigDataAsset* self, const FInstancedStruct& Struct, FMotivatorFormulaVMCode& Slot)
			{
				auto& Op = Struct.Get<FDriverFunctionOperationMul>();
				PushWord(Slot.Opcodes, Op.Instances.Num());
				PushOpcode(Slot.Opcodes, Mul);
				for (auto& elem : Op.Instances)
				{
					self->CompileRec(elem, Slot);
				}
			}
		},
		{
			FDriverFunctionOperationMax::StaticStruct(),
			[](UMotivatorConfigDataAsset* self, const FInstancedStruct& Struct, FMotivatorFormulaVMCode& Slot)
			{
				auto& Op = Struct.Get<FDriverFunctionOperationMax>();
				PushWord(Slot.Opcodes, Op.Instances.Num());
				PushOpcode(Slot.Opcodes, Max);
				for (auto& elem : Op.Instances)
				{
					self->CompileRec(elem, Slot);
				}
			}
		},
		{
			FDriverFunctionOperationMin::StaticStruct(),
			[](UMotivatorConfigDataAsset* self, const FInstancedStruct& Struct, FMotivatorFormulaVMCode& Slot)
			{
				auto& Op = Struct.Get<FDriverFunctionOperationMin>();
				PushWord(Slot.Opcodes, Op.Instances.Num());
				PushOpcode(Slot.Opcodes, Min);
				for (auto& elem : Op.Instances)
				{
					self->CompileRec(elem, Slot);
				}
			}
		},
		{
			FDriverFunctionOperationAverage::StaticStruct(),
			[](UMotivatorConfigDataAsset* self, const FInstancedStruct& Struct, FMotivatorFormulaVMCode& Slot)
			{
				auto& Op = Struct.Get<FDriverFunctionOperationAverage>();
				PushWord(Slot.Opcodes, Op.Instances.Num());
				PushOpcode(Slot.Opcodes, Avg);
				for (auto& elem : Op.Instances)
				{
					self->CompileRec(elem, Slot);
				}
			}
		},
		{
			FDriverFunctionOperationAverageWeighted::StaticStruct(),
			[](UMotivatorConfigDataAsset* self, const FInstancedStruct& Struct, FMotivatorFormulaVMCode& Slot)
			{
				auto& Op = Struct.Get<FDriverFunctionOperationAverageWeighted>();
				for (auto it = Op.Instances.rbegin(); it != Op.Instances.rend(); ++it)
				{
					PushFloat(Slot.Opcodes, (*it).Weight);
				}
				PushWord(Slot.Opcodes, Op.Instances.Num());
				PushOpcode(Slot.Opcodes, AvgW);
				for (auto& elem : Op.Instances)
				{
					self->CompileRec(elem.Instance, Slot);
				}
			}
		},
		{
			nullptr,
			[](UMotivatorConfigDataAsset* self, const FInstancedStruct& Struct, FMotivatorFormulaVMCode& Slot)
			{
				PushOpcode(Slot.Opcodes, Invalid);
			}
			
		}
	};
	auto StructType = Operation.GetScriptStruct();
	auto Processor = Processors.Find(StructType);
	if (ensureMsgf(Processor, TEXT("Unable to generate bytecode for struct [%s]!"), *StructType->GetName()))
	{
		(*Processor)(this, Operation, BytecodeSlot);
	} else
	{
		Processors[nullptr](this, Operation, BytecodeSlot);
	}
}

bool UMotivatorConfigDataAsset::FindGoal(UMotivatorConfigDataAsset* AssetCDO, const FActionStorage& CurrentState,
	FActionPlannerGoal& GoalState)
{
	TMap<FName, float> MentalTraits;
	for (auto& elem : AssetCDO->MentalTraitBytecodes)
	{
		FMotivatorFormulaVMExecutionContext Context(elem.Value, CurrentState.GetAllStates());
		MentalTraits.Add(elem.Key, Context.Execute());
	}

	TPriorityQueue<FMotivatorPossibleResult*> Decisions;
	for (auto& Decision : AssetCDO->Decisions)
	{
		float Utility = 0.0f;
		for (auto& WeightPerTrait : Decision.WeightPerMentalTrait)
		{
			Utility += WeightPerTrait.Value * MentalTraits[WeightPerTrait.Key];
		}
		for (auto& ContextModifier : Decision.ContextModifiersBytecode)
		{
			FMotivatorFormulaVMExecutionContext Context(ContextModifier, CurrentState.GetAllStates());
			Utility += Context.Execute();
		}

		// TODO: Add itertia and noise

		Decisions.Push(&Decision, Utility);
	}

	if (ensure(!Decisions.IsEmpty()))
	{
		auto Best = Decisions.Pop();
		GoalState.State = Best->SetState;
		return true;
	}

	return false;
}

void UMotivatorConfigDataAsset::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	if (SaveContext.IsCooking())
	{
		return;
	}

	MentalTraitBytecodes.Empty();
	for (auto& MentalTraitFunction : MentalTraitFunctions)
	{
		auto BytecodeSlot = MentalTraitBytecodes.Add(MentalTraitFunction.Key);
		Compile(MentalTraitFunction.Value, BytecodeSlot);

		// TODO: Remove
		TSet<FName> CurrentKnowledge;
		FMotivatorFormulaVMExecutionContext ContextForDump(BytecodeSlot, CurrentKnowledge);
		ContextForDump.DumpVM();
	}

	for (auto& Decision : Decisions)
	{
		Decision.ContextModifiersBytecode.Empty();
		Decision.ContextModifiersBytecode.AddZeroed(Decision.ContextModifiers.Num());
		for (size_t i = 0; i < Decision.ContextModifiers.Num(); i++)
		{
			Compile(Decision.ContextModifiers[i], Decision.ContextModifiersBytecode[i]);

			// TODO: Remove
			TSet<FName> CurrentKnowledge;
			FMotivatorFormulaVMExecutionContext ContextForDump(Decision.ContextModifiersBytecode[i], CurrentKnowledge);
			ContextForDump.DumpVM();
		}
	}
}
#endif
