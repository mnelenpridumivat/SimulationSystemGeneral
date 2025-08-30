// Copyright Epic Games, Inc. All Rights Reserved.

#include "SquadDataHandleLayout.h"

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
#include "SimulationSystemSettings.h"

#define LOCTEXT_NAMESPACE "FSquadDataNPCLayout"

void FSquadDataHandleLayout::OnGetAvalableSquads(TArray<TSharedPtr<FString>>& Strings, TArray<TSharedPtr<SToolTip>>& Shareds,
	TArray<bool>& Array)
{
	if (GetDefault<USimulationSystemSettings>()->Squads.IsNull())
	{
		return;
	}
	auto Table = GetDefault<USimulationSystemSettings>()->Squads.LoadSynchronous();
	ensureMsgf(IsValid(Table), TEXT("Unable to load Squads data table!"));
	if (!IsValid(Table))
	{
		return;
	}
	ensureMsgf(Table->RowStruct->IsChildOf(FSquadData::StaticStruct()), TEXT("Squads data table must be a struct child of FSquadData!"));
	if (!Table->RowStruct->IsChildOf(FSquadData::StaticStruct()))
	{
		return;
	}
	TSet<FName> SquadNames = {};
	Table->ForeachRow<FSquadData>("", [&](const FName& Key, const FSquadData& Value)
	{
		SquadNames.Add(Key);
	});
	for (const FName& Name : SquadNames)
	{
		Strings.Add(MakeShared<FString>(Name.ToString()));
	}
}

void FSquadDataHandleLayout::CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	this->StructPropertyHandle = InStructPropertyHandle;
	
	HeaderRow
	.NameContent()
	[
		InStructPropertyHandle->CreatePropertyNameWidget(FText::GetEmpty(), FText::GetEmpty(), false)
	];
}

void FSquadDataHandleLayout::CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	auto ChildHandle = InStructPropertyHandle->GetChildHandle("Name");
	
	if (ChildHandle->IsValidHandle())
	{

		FPropertyComboBoxArgs ComboBoxArgs;
		ComboBoxArgs.PropertyHandle = ChildHandle;
		ComboBoxArgs.ShowSearchForItemCount = 1;
		ComboBoxArgs.OnGetStrings = FOnGetPropertyComboBoxStrings::CreateSP(this, &FSquadDataHandleLayout::OnGetAvalableSquads);
		//ComboBoxArgs.OnGetValue = FOnGetPropertyComboBoxValue::CreateSP(this, &FSquadDataNPCLayout::OnGetNPCValue);
		
		StructBuilder.AddCustomRow(LOCTEXT("Squad_Name", "Squad Name"))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Squad_Name", "Squad Name"))
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

