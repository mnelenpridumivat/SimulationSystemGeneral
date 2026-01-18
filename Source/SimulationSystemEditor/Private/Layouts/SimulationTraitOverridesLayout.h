// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakObjectPtr.h"
#include "Layout/Visibility.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"

class FSimulationTraitOverridesLayout : public IPropertyTypeCustomization
{
	TSharedPtr<IPropertyHandle> StructPropertyHandle;
	TSharedPtr<IPropertyHandle> CanRandomPropertyHandle;
	TSharedPtr<IPropertyHandle> TablePropertyHandle;
	TSharedPtr<IPropertyHandle> UseRandomPropertyHandle;
	TSharedPtr<IPropertyHandle> RowPropertyHandle;

	void OnGetAvalableRows(TArray<TSharedPtr<FString>>& Strings, TArray<TSharedPtr<SToolTip>>& Shareds, TArray<bool>& Array);
	void OnUseRandomChanged();
	
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance() 
	{
		return MakeShareable( new FSimulationTraitOverridesLayout );
	}

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	
};
