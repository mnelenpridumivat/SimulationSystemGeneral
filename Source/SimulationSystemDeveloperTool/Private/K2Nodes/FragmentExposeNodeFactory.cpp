#include "FragmentExposeNodeFactory.h"

#include "K2Node_FragmentExpose.h"
#include "Slate/GraphNodeFragmentExpose.h"

TSharedPtr<SGraphNode> FFragmentExposeNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (Node && Node->IsA<UK2Node_FragmentExpose>())
	{
		UK2Node* K2Node = static_cast<UK2Node*>(Node);
		return SNew(SGraphNodeFragmentExpose, K2Node);
	}
	return FGraphPanelNodeFactory::CreateNode(Node);
}
