// Copyright Epic Games, Inc. All Rights Reserved.

#include "EntityDataHandleLayout.h"

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
#include "Tables/SimulationArchetypeSettings.h"

#define LOCTEXT_NAMESPACE "FEntityDataNPCLayout"

void FEntityDataHandleLayout::OnGetAvalableEntities(TArray<TSharedPtr<FString>>& Strings, TArray<TSharedPtr<SToolTip>>& Shareds,
	TArray<bool>& Array)
{
	if (GetDefault<USimulationSystemSettings>()->AllEntities.IsNull())
	{
		return;
	}
	auto Table = GetDefault<USimulationSystemSettings>()->AllEntities.LoadSynchronous();
	if (!ensureMsgf(IsValid(Table), TEXT("Unable to load Entities data table!")))
	{
		return;
	}
	if (!ensureMsgf(Table->RowStruct->IsChildOf(FSimulationArchetypeSettings::StaticStruct()),
	                TEXT("Entities data table must be a struct child of FSimulationArchetypeSettings!")))
	{
		return;
	}
	TSet<FName> EntitiesNames = {};
	Table->ForeachRow<FSimulationArchetypeSettings>("", [&](const FName& Key, const FSimulationArchetypeSettings& Value)
	{
		EntitiesNames.Add(Key);
	});
	for (const FName& Name : EntitiesNames)
	{
		Strings.Add(MakeShared<FString>(Name.ToString()));
	}
}

void FEntityDataHandleLayout::CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	this->StructPropertyHandle = InStructPropertyHandle;
	
	HeaderRow
	.NameContent()
	[
		InStructPropertyHandle->CreatePropertyNameWidget(FText::GetEmpty(), FText::GetEmpty())
	];
}

void FEntityDataHandleLayout::CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	auto ChildHandle = InStructPropertyHandle->GetChildHandle("Name");
	
	if (ChildHandle->IsValidHandle())
	{

		FPropertyComboBoxArgs ComboBoxArgs;
		ComboBoxArgs.PropertyHandle = ChildHandle;
		ComboBoxArgs.ShowSearchForItemCount = 1;
		ComboBoxArgs.OnGetStrings = FOnGetPropertyComboBoxStrings::CreateSP(this, &FEntityDataHandleLayout::OnGetAvalableEntities);
		//ComboBoxArgs.OnGetValue = FOnGetPropertyComboBoxValue::CreateSP(this, &FSquadDataNPCLayout::OnGetNPCValue);
		
		StructBuilder.AddCustomRow(LOCTEXT("Entity_Name", "Entity Name"))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Entity_Name", "Entity Name"))
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

