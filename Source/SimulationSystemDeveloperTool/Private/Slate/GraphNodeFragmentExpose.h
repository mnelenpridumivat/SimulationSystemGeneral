#pragma once
#include "SGraphNode.h"
#include "KismetNodes/SGraphNodeK2Default.h"

class SGraphNodeFragmentExpose : public SGraphNodeK2Default
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeFragmentExpose) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UK2Node* InGraphNode);
	
protected:
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	
};
