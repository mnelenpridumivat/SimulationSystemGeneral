// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimulationDataTableKeyLayout.h"

#include "AutoKeyRegistration.h"
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
#include "SimulationSquadHandlerTrait.h"
#include "SimulationSystemSettings.h"

#define LOCTEXT_NAMESPACE "FSimulationDataTableKeyLayout"

void FSimulationDataTableKeyLayout::OnGetAvalableSquads(TArray<TSharedPtr<FString>>& Strings, TArray<TSharedPtr<SToolTip>>& Shareds,
	TArray<bool>& Array)
{
	TSet<FName>& Keys = GetMutableDefault<USimulationSystemSettings>()->GetEntitiesTableKeys();
	for (const FName& Name : Keys)
	{
		Strings.Add(MakeShared<FString>(Name.ToString()));
	}
}

void FSimulationDataTableKeyLayout::CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	this->StructPropertyHandle = InStructPropertyHandle;
	
	HeaderRow
	.NameContent()
	[
		InStructPropertyHandle->CreatePropertyNameWidget(FText::GetEmpty(), FText::GetEmpty(), false)
	];
}

void FSimulationDataTableKeyLayout::CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	auto ChildHandle = InStructPropertyHandle->GetChildHandle("Key");
	
	if (ChildHandle->IsValidHandle())
	{

		FPropertyComboBoxArgs ComboBoxArgs;
		ComboBoxArgs.PropertyHandle = ChildHandle;
		ComboBoxArgs.ShowSearchForItemCount = 1;
		ComboBoxArgs.OnGetStrings = FOnGetPropertyComboBoxStrings::CreateSP(this, &FSimulationDataTableKeyLayout::OnGetAvalableSquads);
		
		StructBuilder.AddCustomRow(LOCTEXT("Key_Name", "Key"))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Key_Name", "Key Name"))
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

