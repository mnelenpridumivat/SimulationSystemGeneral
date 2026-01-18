#include "GraphPinNameComboBox.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"

void SGraphPinNameComboBox::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);

	NameOptions.Empty();
	FillOptions();

	if (!ensure(!NameOptions.IsEmpty()))
	{
		return;
	}

	for (auto& Name : NameOptions)
	{
		if (*Name == InPin->DefaultValue)
		{
			SelectedName = Name;
		}
	}

	if (!SelectedName.IsValid())
	{
		SelectedName = NameOptions.Last();
	}
}

TSharedRef<SWidget> SGraphPinNameComboBox::GetDefaultValueWidget()
{
	return SNew(SComboBox<TSharedPtr<FName>>)
		.OptionsSource(&NameOptions)
		.OnSelectionChanged(this, &SGraphPinNameComboBox::OnNameSelected)
		.OnGenerateWidget(this, &SGraphPinNameComboBox::MakeNameItemWidget)
		.InitiallySelectedItem(SelectedName)
		.Content()
		[
			SNew(STextBlock)
			.Text_Lambda([this]()
			{
				return SelectedName.IsValid() ? FText::FromName(*SelectedName) : FText::FromString("None");
			})
		];
}

void SGraphPinNameComboBox::OnNameSelected(TSharedPtr<FName> InSelectedName, ESelectInfo::Type InSelectInfo)
{
	if (InSelectedName.IsValid())
	{
		SelectedName = InSelectedName;
		auto Pin = GetPinObj();
		if (ensure(Pin))
		{
			Pin->GetSchema()->TrySetDefaultValue(*Pin, SelectedName->ToString());
		}
	}
}

TSharedRef<SWidget> SGraphPinNameComboBox::MakeNameItemWidget(TSharedPtr<FName> InSelectedName) const
{
	return SNew(STextBlock).Text(FText::FromName(*InSelectedName));
}
