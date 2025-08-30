// Copyright Epic Games, Inc. All Rights Reserved.

#include "SquadDataNPCLayout.h"

#include "DataTableEditorUtils.h"
#include "ScopedTransaction.h"
#include "Engine/EngineBaseTypes.h"
#include "UObject/UnrealType.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "DetailWidgetRow.h"
#include "IDetailsView.h"
#include "EdGraphSchema_K2.h"
#include "EditorClassUtils.h"
#include "IDetailChildrenBuilder.h"
#include "ProfileGeneratorNPCBase.h"
#include "ProfileGeneratorSquad.h"
#include "Widgets/SToolTip.h"
#include "PropertyCustomizationHelpers.h"
#include "SimulationSystemEditorSubsystem.h"
#include "SimulationSystemSettings.h"

#define LOCTEXT_NAMESPACE "FSquadDataNPCLayout"

void FSquadDataNPCLayout::OnGetAvalableNPCs(TArray<TSharedPtr<FString>>& Strings, TArray<TSharedPtr<SToolTip>>& Shareds,
	TArray<bool>& Array)
{
	auto Subsystem = GEditor->GetEditorSubsystem<USimulationSystemEditorSubsystem>();
	TArray<FName> Classes;
	Subsystem->GetAllPawnClasses(Classes);
	
	for (const FName& Class : Classes)
	{
		Strings.Add(MakeShared<FString>(Class.ToString()));
	}
}

void FSquadDataNPCLayout::CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	this->StructPropertyHandle = InStructPropertyHandle;
	
	HeaderRow
	.NameContent()
	[
		InStructPropertyHandle->CreatePropertyNameWidget(FText::GetEmpty(), FText::GetEmpty(), false)
	];
}

void FSquadDataNPCLayout::CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	auto ChildHandle = InStructPropertyHandle->GetChildHandle("Name");
	
	if (ChildHandle->IsValidHandle())
	{

		FPropertyComboBoxArgs ComboBoxArgs;
		ComboBoxArgs.PropertyHandle = ChildHandle;
		ComboBoxArgs.ShowSearchForItemCount = 1;
		ComboBoxArgs.OnGetStrings = FOnGetPropertyComboBoxStrings::CreateSP(this, &FSquadDataNPCLayout::OnGetAvalableNPCs);
		//ComboBoxArgs.OnGetValue = FOnGetPropertyComboBoxValue::CreateSP(this, &FSquadDataNPCLayout::OnGetNPCValue);
		
		StructBuilder.AddCustomRow(LOCTEXT("NPC_Class", "NPC Class"))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("NPC_Class", "NPC Class"))
			.Font(StructCustomizationUtils.GetRegularFont())
		]
		.ValueContent()
		.MaxDesiredWidth(0.0f)
		[
			PropertyCustomizationHelpers::MakePropertyComboBox(ComboBoxArgs)
		];
	}
}

#undef LOCTEXT_NAMESPACE

