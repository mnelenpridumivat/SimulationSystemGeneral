// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_FragmentExpose.h"

#include "ActionPlannerOwner.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "K2Node_ClassDynamicCast.h"
#include "KismetCompiler.h"
#include "SimulationFunctionLibrary.h"
#include "Kismet/KismetStringLibrary.h"

void UK2Node_FragmentExpose::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions(ActionRegistrar);

	if (const UClass* Action = GetClass(); ActionRegistrar.IsOpenForRegistration(Action)) {
		UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(GetClass());
		check(Spawner != nullptr);

		ActionRegistrar.AddBlueprintAction(Action, Spawner);
	}
}

void UK2Node_FragmentExpose::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::PinDefaultValueChanged(Pin);

	if (Pin && Pin->PinName == StructPropertyName)
	{
		StructName = Pin->DefaultValue;
		ReconstructNode();
		GetGraph()->NotifyGraphChanged();
	}
}

void UK2Node_FragmentExpose::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, {}, UActionPlannerOwner::StaticClass(), OwnerPropertyName);
	auto Pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, {}, StructPropertyName);
	Pin->DefaultValue = StructName;
	Pin->bNotConnectable = true;
	
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, {}, HasResultPropertyName);
	Struct = USimulationFunctionLibrary::GetStructFromName(StructName);
	if (!Struct)
	{
		return;
	}
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	for (TFieldIterator<FProperty> Iterator(Struct); Iterator; ++Iterator)
	{
		auto Property = *Iterator;
		FEdGraphPinType Type;
		if (Property->HasAnyPropertyFlags(CPF_BlueprintVisible)
			&& ensure(K2Schema->ConvertPropertyToPinType(Property, Type)))
		{
			CreatePin(EGPD_Output, Type, Property->NamePrivate);
		}
	}
}

void UK2Node_FragmentExpose::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	auto OwnerPin = FindPinChecked(OwnerPropertyName, EGPD_Input);
	auto StructPin = FindPinChecked(StructPropertyName, EGPD_Input);
	StructName = StructPin->DefaultValue;
	auto HasResultPin = FindPinChecked(HasResultPropertyName, EGPD_Output);

	auto GetFragmentFunc = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	GetFragmentFunc->SetFromFunction(
		USimulationFunctionLibrary::StaticClass()->
		FindFunctionByName(
			GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetConstFragmentData)
			)
		);
	GetFragmentFunc->AllocateDefaultPins();

	CompilerContext.MovePinLinksToIntermediate(
		*OwnerPin,
		*GetFragmentFunc->FindPin(OwnerPropertyName, EGPD_Input));
	CompilerContext.MovePinLinksToIntermediate(
		*HasResultPin,
		*GetFragmentFunc->GetReturnValuePin());
	auto FragmentHandlerPin = GetFragmentFunc->FindPin(OutPropertyName, EGPD_Output);
	
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	int Index = 0;
	for (TFieldIterator<FProperty> Iterator(Struct); Iterator; ++Iterator)
	{
		auto Property = *Iterator;
		FEdGraphPinType Type;
		if (Property->HasAnyPropertyFlags(CPF_BlueprintVisible)
			&& ensure(K2Schema->ConvertPropertyToPinType(Property, Type)))
		{
			bool NeedObjCast = false;
			FName SpecialReturnPin = NAME_None;
			FName Func = NAME_None;
			if (Type.PinCategory == UEdGraphSchema_K2::PC_Int)
			{
				Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetIntFragmentProperty);
			} else if (Type.PinCategory == UEdGraphSchema_K2::PC_Float)
			{
				Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetFloatFragmentProperty);
			} else if (Type.PinCategory == UEdGraphSchema_K2::PC_Double)
			{
				Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetDoubleFragmentProperty);
			} else if (Type.PinCategory == UEdGraphSchema_K2::PC_Real)
			{
				if (Type.PinSubCategory == UEdGraphSchema_K2::PC_Float)
				{
					Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetFloatFragmentProperty);
				} else if (Type.PinSubCategory == UEdGraphSchema_K2::PC_Double)
				{
					Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetDoubleFragmentProperty);
				} else
				{
					ensure(false);
					continue;
				}
			} else if (Type.PinCategory == UEdGraphSchema_K2::PC_Boolean)
			{
				Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetBoolFragmentProperty);
			} else if (Type.PinCategory == UEdGraphSchema_K2::PC_String)
			{
				Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetStringFragmentProperty);
			} else if (Type.PinCategory == UEdGraphSchema_K2::PC_Name)
			{
				Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetNameFragmentProperty);
			} else if (Type.PinCategory == UEdGraphSchema_K2::PC_Text)
			{
				Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetTextFragmentProperty);
			} else if (Type.PinCategory == UEdGraphSchema_K2::PC_Object)
			{
				NeedObjCast = true;
				Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetObjectFragmentProperty);
			} else if (Type.PinCategory == UEdGraphSchema_K2::PC_Class)
			{
				NeedObjCast = true;
				Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetClassFragmentProperty);
			} else if (Type.PinCategory == UEdGraphSchema_K2::PC_Struct)
			{
				Func = GET_MEMBER_NAME_CHECKED(USimulationFunctionLibrary, GetStructFragmentProperty);
				SpecialReturnPin = OutPropertyName;
			}
			if (!ensure(Func != NAME_None))
			{
				continue;
			}
			
			auto FragmentConvFunc = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
			FragmentConvFunc->SetFromFunction(
				USimulationFunctionLibrary::StaticClass()->FindFunctionByName(Func)
			);
			FragmentConvFunc->AllocateDefaultPins();

			auto FragmentConvInPin = FragmentConvFunc->FindPin(MassFragmentPropertyName, EGPD_Input);
			FragmentHandlerPin->MakeLinkTo(FragmentConvInPin);
			auto FragmentConvIndexPin = FragmentConvFunc->FindPin(IndexPropertyName, EGPD_Input);
			FragmentConvIndexPin->DefaultValue = UKismetStringLibrary::Conv_IntToString(Index);
			auto FragmentConvOutPin = SpecialReturnPin == NAME_None ? FragmentConvFunc->GetReturnValuePin() : FragmentConvFunc->FindPinChecked(SpecialReturnPin, EGPD_Output);

			if (NeedObjCast)
			{
				if (Type.PinCategory == UEdGraphSchema_K2::PC_Object)
				{
					auto CastProp = CastField<FObjectProperty>(Property);
					auto CastNode = CompilerContext.SpawnIntermediateNode<UK2Node_DynamicCast>(this, SourceGraph);
					CastNode->SetPurity(true);
					CastNode->TargetType = CastProp->PropertyClass;
					CastNode->AllocateDefaultPins();
					auto CastInputPin = CastNode->GetCastSourcePin();
					CastInputPin->PinType = FragmentConvOutPin->PinType;
					FragmentConvOutPin->MakeLinkTo(CastInputPin);
					FragmentConvOutPin = CastNode->GetCastResultPin();
				} else if (Type.PinCategory == UEdGraphSchema_K2::PC_Class)
				{
					auto CastProp = CastField<FClassProperty>(Property);
					auto CastNode = CompilerContext.SpawnIntermediateNode<UK2Node_ClassDynamicCast>(this, SourceGraph);
					CastNode->SetPurity(true);
					CastNode->TargetType = CastProp->MetaClass;
					CastNode->AllocateDefaultPins();
					FragmentConvOutPin->MakeLinkTo(CastNode->GetCastSourcePin());
					FragmentConvOutPin = CastNode->GetCastResultPin();
				}
			}

			auto OutPin = FindPinChecked(Property->NamePrivate, EGPD_Output);
			CompilerContext.MovePinLinksToIntermediate(*OutPin, *FragmentConvOutPin);

			++Index;
		}
	}

	BreakAllNodeLinks();
}

FText UK2Node_FragmentExpose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_FragmentExpose", "UK2Node_FragmentExpose_Name", "Fragment Expose");
}
