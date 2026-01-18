#pragma once

#include "SGraphPin.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

class SGraphPinNameComboBox : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SGraphPinNameComboBox) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:

	TArray<TSharedPtr<FName>> NameOptions;
	TSharedPtr<FName> SelectedName;

	virtual TSharedRef<SWidget> GetDefaultValueWidget() override;
	virtual void FillOptions() = 0;

	void OnNameSelected(TSharedPtr<FName> InSelectedName, ESelectInfo::Type InSelectInfo);
	TSharedRef<SWidget> MakeNameItemWidget(TSharedPtr<FName> InSelectedName) const;
	
};
