#include "GraphNodeFragmentExpose.h"

#include "GraphPinFragmentNameComboBox.h"
#include "K2Node_FragmentExpose.h"

void SGraphNodeFragmentExpose::Construct(const FArguments& InArgs, UK2Node* InGraphNode)
{
	SGraphNodeK2Default::Construct(SGraphNodeK2Default::FArguments(), InGraphNode);
}

TSharedPtr<SGraphPin> SGraphNodeFragmentExpose::CreatePinWidget(UEdGraphPin* Pin) const
{
	if (Pin->PinName == UK2Node_FragmentExpose::StructPropertyName)
	{
		return SNew(SGraphPinFragmentNameComboBox, Pin);
	}
	return SGraphNode::CreatePinWidget(Pin);
}
