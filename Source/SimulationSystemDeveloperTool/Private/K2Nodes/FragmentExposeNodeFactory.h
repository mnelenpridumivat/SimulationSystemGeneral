#pragma once
#include "EdGraphUtilities.h"

class FFragmentExposeNodeFactory : public FGraphPanelNodeFactory
{
public:
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;
	
};
