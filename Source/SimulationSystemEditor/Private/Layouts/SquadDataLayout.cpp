// Copyright Epic Games, Inc. All Rights Reserved.

#include "SquadDataLayout.h"

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
#include "ProfileGeneratorSquad.h"
#include "Widgets/SToolTip.h"
#include "PropertyCustomizationHelpers.h"

#define LOCTEXT_NAMESPACE "FSquadDataLayout"

void FSquadDataLayout::CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}

void FSquadDataLayout::CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}

#undef LOCTEXT_NAMESPACE

