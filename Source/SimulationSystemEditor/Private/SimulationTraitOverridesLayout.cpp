// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimulationTraitOverridesLayout.h"

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
#include "SimulationTableTrait.h"
#include "Tables/SimulationArchetypeSettings.h"

#define LOCTEXT_NAMESPACE "FSimulationTraitOverridesLayout"

void FSimulationTraitOverridesLayout::OnGetAvalableRows(TArray<TSharedPtr<FString>>& Strings, TArray<TSharedPtr<SToolTip>>& Shareds,
	TArray<bool>& Array)
{
	TSubclassOf<USimulationTableTrait> TraitClass;
	{
		UObject* Temp = nullptr;
		if (!ensure(TablePropertyHandle->GetValue(Temp) == FPropertyAccess::Success))
		{
			return;
		}
		TraitClass = Cast<UClass>(Temp);
		if (!ensure(TraitClass))
		{
			return;
		}
	}
	bool UseRandom;
	if (!ensure(UseRandomPropertyHandle->GetValue(UseRandom) == FPropertyAccess::Success))
	{
		return;
	}

	UDataTable* DataTable;
	if (UseRandom)
	{
		DataTable = TraitClass->GetDefaultObject<USimulationTableTrait>()->GetRandomsDataTable();
	} else
	{
		DataTable = TraitClass->GetDefaultObject<USimulationTableTrait>()->GetDefaultsDataTable();
	}
	if (!IsValid(DataTable))
	{
		return;
	}
	TArray<FName> EntitiesNames = DataTable->GetRowNames();
	for (const FName& Name : EntitiesNames)
	{
		Strings.Add(MakeShared<FString>(Name.ToString()));
	}
}

void FSimulationTraitOverridesLayout::OnUseRandomChanged()
{
	if (!ensure(RowPropertyHandle->IsValidHandle()))
	{
		return;
	}
	RowPropertyHandle->SetValue(NAME_None);
}

void FSimulationTraitOverridesLayout::CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	StructPropertyHandle = InStructPropertyHandle;
	CanRandomPropertyHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimulationTraitOverrides, CanBeRandom));
	TablePropertyHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimulationTraitOverrides, Trait));
	UseRandomPropertyHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimulationTraitOverrides, UseRandom));
	RowPropertyHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimulationTraitOverrides, RowName));
	
	HeaderRow
	.NameContent()
	[
		InStructPropertyHandle->CreatePropertyNameWidget(FText::GetEmpty(), FText::GetEmpty(), false)
	];
}

void FSimulationTraitOverridesLayout::CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	if (CanRandomPropertyHandle->IsValidHandle())
	{
		StructBuilder.AddProperty(CanRandomPropertyHandle.ToSharedRef());
	}
	if (RowPropertyHandle->IsValidHandle())
	{
		FPropertyComboBoxArgs ComboBoxArgs;
		ComboBoxArgs.PropertyHandle = RowPropertyHandle;
		ComboBoxArgs.ShowSearchForItemCount = 1;
		ComboBoxArgs.OnGetStrings = FOnGetPropertyComboBoxStrings::CreateSP(this, &FSimulationTraitOverridesLayout::OnGetAvalableRows);
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
	if (UseRandomPropertyHandle->IsValidHandle())
	{
		StructBuilder.AddProperty(UseRandomPropertyHandle.ToSharedRef());
		UseRandomPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSimulationTraitOverridesLayout::OnUseRandomChanged));
	}
}

#undef LOCTEXT_NAMESPACE

