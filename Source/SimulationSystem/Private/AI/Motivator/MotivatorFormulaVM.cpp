#include "MotivatorFormulaVM.h"

#include "Kismet/KismetStringLibrary.h"

DEFINE_LOG_CATEGORY(LogMotivatorVMState);

bool FMotivatorFormulaVMOpcode::Serialize(FArchive& Ar)
{
	auto Index = Opcode.GetIndex();
	Ar << Index;

	if (!Ar.IsLoading())
	{
		switch (Index)
		{
		case Opcode.IndexOfType<EMotivatorFormulaVMOpcodeType>():
			{
				std::underlying_type_t<EMotivatorFormulaVMOpcodeType> Val;
				if (!Ar.IsLoading())
				{
					Val = Opcode.Get<EMotivatorFormulaVMOpcodeType>();
				}
				Ar << Val;
				if (Ar.IsLoading())
				{
					Opcode.Set<EMotivatorFormulaVMOpcodeType>((EMotivatorFormulaVMOpcodeType)Val);
				}
				break;
			}
		case Opcode.IndexOfType<float>():
			{
				float Val;
				if (!Ar.IsLoading())
				{
					Val = Opcode.Get<float>();
				}
				Ar << Val;
				if (Ar.IsLoading())
				{
					Opcode.Set<float>(Val);
				}
				break;
			}
		case Opcode.IndexOfType<FName>():
			{
				FName Val;
				if (!Ar.IsLoading())
				{
					Val = Opcode.Get<FName>();
				}
				Ar << Val;
				if (Ar.IsLoading())
				{
					Opcode.Set<FName>(Val);
				}
				break;
			}
		case Opcode.IndexOfType<uint16>():
			{
				uint16 Val;
				if (!Ar.IsLoading())
				{
					Val = Opcode.Get<uint16>();
				}
				Ar << Val;
				if (Ar.IsLoading())
				{
					Opcode.Set<uint16>(Val);
				}
				break;
			}
		default:
			{
				check(false);
			}
		}
	} else
	{
		
	}

	return true;
}

FMotivatorFormulaVMExecutionContext::FMotivatorFormulaVMExecutionContext(FMotivatorFormulaVMCode& NewCode,
                                                                         const TSet<FName>& NewKnowledge)
{
	Code = &NewCode;
	CurrentKnowledge = &NewKnowledge;
	CurrentCodeIndex = NewCode.Opcodes.Num()-1;
}

float FMotivatorFormulaVMExecutionContext::Execute()
{
	check(Code);
	if (!ensure(Code->IsValid))
	{
		return 0.0f;
	}
	check(CurrentKnowledge);
	check(Code->Opcodes.IsValidIndex(CurrentCodeIndex));

	static auto ConstStackGetOpcode = [this](TArray<FMotivatorFormulaVMOpcode>& Code, size_t& CurrentIndex) -> EMotivatorFormulaVMOpcodeType
	{
		auto& Opcode = Code[CurrentIndex--];
		if (!Opcode.Opcode.IsType<EMotivatorFormulaVMOpcodeType>())
		{
			DumpVM();
			check(Opcode.Opcode.IsType<EMotivatorFormulaVMOpcodeType>());
		}
		return Opcode.Opcode.Get<EMotivatorFormulaVMOpcodeType>();
	};
	static auto ConstStackGetName = [this](TArray<FMotivatorFormulaVMOpcode>& Code, size_t& CurrentIndex) -> FName
	{
		auto& Opcode = Code[CurrentIndex--];
		if (!Opcode.Opcode.IsType<FName>())
		{
			DumpVM();
			check(Opcode.Opcode.IsType<FName>());
		}
		return Opcode.Opcode.Get<FName>();
	};
	static auto ConstStackGetWord = [this](TArray<FMotivatorFormulaVMOpcode>& Code, size_t& CurrentIndex) -> uint16
	{
		auto& Opcode = Code[CurrentIndex--];
		if (!Opcode.Opcode.IsType<uint16>())
		{
			DumpVM();
			check(Opcode.Opcode.IsType<uint16>());
		}
		return Opcode.Opcode.Get<uint16>();
	};
	static auto ConstStackGetFloat = [this](TArray<FMotivatorFormulaVMOpcode>& Code, size_t& CurrentIndex) -> float
	{
		auto& Opcode = Code[CurrentIndex--];
		if (!Opcode.Opcode.IsType<float>())
		{
			DumpVM();
			check(Opcode.Opcode.IsType<float>());
		}
		return Opcode.Opcode.Get<float>();
	};
	

	while (CurrentCodeIndex)
	{
		auto Op = ConstStackGetOpcode(Code->Opcodes, CurrentCodeIndex);
		switch (Op)
		{
		case Invalid:
			{
				checkf(false, TEXT("Encountered invalid opcode during interpretation!"));
				break;
			}
		case PushParam:
			{
				auto ParamName = ConstStackGetName(Code->Opcodes, CurrentCodeIndex);
				auto ParamIndex = ConstStackGetWord(Code->Opcodes, CurrentCodeIndex);
				auto& ConvData = Code->Conversions[ParamName].Variants[ParamIndex];
				Stack.Push(CurrentKnowledge->Contains(ParamName) ? ConvData.ValueHas : ConvData.ValueDontHas);
				break;
			}
		case ClampDefault:
			{
				auto Min = ConstStackGetFloat(Code->Opcodes, CurrentCodeIndex);
				auto Max = ConstStackGetFloat(Code->Opcodes, CurrentCodeIndex);
				auto RawValue = Stack.Pop();
				Stack.Push(FMath::Clamp(RawValue, Min, Max));
				break;
			}
		case ClampSigmoid:
			{
				auto Min = ConstStackGetFloat(Code->Opcodes, CurrentCodeIndex);
				auto Max = ConstStackGetFloat(Code->Opcodes, CurrentCodeIndex);
				auto Steepness = ConstStackGetFloat(Code->Opcodes, CurrentCodeIndex);
				auto Center = ConstStackGetFloat(Code->Opcodes, CurrentCodeIndex);
				auto RawValue = Stack.Pop();
				RawValue = 1.0f/(1.0f + FMath::Exp(-Steepness*(RawValue-Center)));
				Stack.Push(Min + RawValue*(Max - Min));
				break;
			}
		case NegateSimple:
			{
				auto RawValue = Stack.Pop();
				Stack.Push(-RawValue);
				break;
			}
		case NegateDefault:
			{
				auto A = ConstStackGetFloat(Code->Opcodes, CurrentCodeIndex);
				auto B = ConstStackGetFloat(Code->Opcodes, CurrentCodeIndex);
				auto RawValue = Stack.Pop();
				Stack.Push(A-RawValue*B);
				break;
			}
		case Sum:
			{
				auto N = ConstStackGetWord(Code->Opcodes, CurrentCodeIndex);
				float Result = 0.0f;
				for (uint16 i = 0; i < N; i++)
				{
					Result += Stack.Pop();
				}
				Stack.Push(Result);
				break;
			}
		case Mul:
			{
				auto N = ConstStackGetWord(Code->Opcodes, CurrentCodeIndex);
				float Result = 1.0f;
				for (uint16 i = 0; i < N; i++)
				{
					Result *= Stack.Pop();
				}
				Stack.Push(Result);
				break;
			}
		case Min:
			{
				auto N = ConstStackGetWord(Code->Opcodes, CurrentCodeIndex);
				float MinValue = MAX_FLT;
				for (uint16 i = 0; i < N; i++)
				{
					auto Temp = Stack.Pop();
					if (Temp < MinValue)
					{
						MinValue = Temp;
					}
				}
				Stack.Push(MinValue);
				break;
			}
		case Max:
			{
				auto N = ConstStackGetWord(Code->Opcodes, CurrentCodeIndex);
				float MaxValue = -MAX_FLT;
				for (uint16 i = 0; i < N; i++)
				{
					auto Temp = Stack.Pop();
					if (Temp > MaxValue)
					{
						MaxValue = Temp;
					}
				}
				Stack.Push(MaxValue);
				break;
			}
		case Avg:
			{
				auto N = ConstStackGetWord(Code->Opcodes, CurrentCodeIndex);
				float SumValue = 0.0f;
				for (uint16 i = 0; i < N; i++)
				{
					SumValue += Stack.Pop();
				}
				Stack.Push(SumValue/N);
				break;
			}
		case AvgW:
			{
				auto N = ConstStackGetWord(Code->Opcodes, CurrentCodeIndex);
				float WeightedSum = 0.0f;
				for (uint16 i = 0; i < N; i++)
				{
					WeightedSum = Stack.Pop() * ConstStackGetFloat(Code->Opcodes, CurrentCodeIndex);
				}
				Stack.Push(WeightedSum/N);
				break;
			}
		default:
			{
				DumpVM();
				checkf(false, TEXT("Invalid opcode [%s]!"), *StaticEnum<EMotivatorFormulaVMOpcodeType>()->GetNameStringByValue(Op))
			}
		}
	}

	if (Stack.Num() != 1)
	{
		DumpVM();
		check(Stack.Num() == 1);
	}
	return Stack.Pop();
}

void FMotivatorFormulaVMExecutionContext::DumpVM()
{
#if !UE_BUILD_SHIPPING
#define VM_LOG(text, ...) UE_LOG(LogMotivatorVMState, Display, text, ##__VA_ARGS__)
	VM_LOG(TEXT("Motivator VM dump:"));

	VM_LOG(TEXT("Stack size: %d"), Stack.Num());
	VM_LOG(TEXT("Begin stack:["));
	for (auto it = Stack.rbegin(); it != Stack.rend(); ++it)
	{
		VM_LOG(TEXT("	%f"), *it);
	}
	VM_LOG(TEXT("]"));

	if (!Code)
	{
		VM_LOG(TEXT("Code is null"));
	} else
	{
		VM_LOG(TEXT("Code valid: %s"), Code->IsValid ? TEXT("true") : TEXT("false"));
		VM_LOG(TEXT("Code size: %d"), Code->Opcodes.Num());
		VM_LOG(TEXT("Current pos: %d"), CurrentCodeIndex);
		VM_LOG(TEXT("Begin Code:["));
		for (size_t i = 0; i < Code->Opcodes.Num(); ++i)
		{
			auto& elem = Code->Opcodes[i];
			FString Line = "	[";
			switch (elem.Opcode.GetIndex())
			{
			case elem.Opcode.IndexOfType<EMotivatorFormulaVMOpcodeType>():
				{
					Line += TEXT("Opcode] ");
					Line += StaticEnum<EMotivatorFormulaVMOpcodeType>()->GetNameStringByValue(elem.Opcode.Get<EMotivatorFormulaVMOpcodeType>());
					break;
				}
			case elem.Opcode.IndexOfType<float>():
				{
					Line += TEXT("float ] ");
					Line += FString::SanitizeFloat(elem.Opcode.Get<float>());
					break;
				}
			case elem.Opcode.IndexOfType<FName>():
				{
					Line += TEXT("FName ] ");
					Line += elem.Opcode.Get<FName>().ToString();
					break;
				}
			case elem.Opcode.IndexOfType<uint16>():
				{
					Line += TEXT("uint16] ");
					Line += FString::FromInt(elem.Opcode.Get<uint16>());
					break;
				}
			default:
				{
					Line += TEXT("error ]");
				}
			}
			if (i == CurrentCodeIndex)
			{
				Line += TEXT(" <- Current Position");
			}
			VM_LOG(TEXT("%s"), *Line);
		}
		VM_LOG(TEXT("]"));

		VM_LOG(TEXT("Begin Convertion table:["));
		for (auto& elem : Code->Conversions)
		{
			VM_LOG(TEXT("Param [%s]:"), *elem.Key.ToString());
			for (int i = 0; i < elem.Value.Variants.Num(); i++)
			{
				auto& span = elem.Value.Variants[i];
				VM_LOG(TEXT("	[%d]: has = %f, dont had = %f"), i, span.ValueHas, span.ValueDontHas);
			}
		}
		VM_LOG(TEXT("]"));
	}

	if (!CurrentKnowledge)
	{
		VM_LOG(TEXT("Current Knowledge is null!"));
	} else
	{
		VM_LOG(TEXT("Current knowledge: ["));
		for (auto& elem : *CurrentKnowledge)
		{
			VM_LOG(TEXT("	%s"), *elem.ToString())
		}
		VM_LOG(TEXT("]"));
	}
	
	VM_LOG(TEXT("End motivator VM dump"));
#undef VM_LOG
#endif
}
