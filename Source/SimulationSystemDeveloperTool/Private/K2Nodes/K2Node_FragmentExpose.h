// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_FragmentExpose.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEMDEVELOPERTOOL_API UK2Node_FragmentExpose : public UK2Node
{
	GENERATED_BODY()
public:
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	
	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual bool IsNodePure() const override {return true;}
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;


	UPROPERTY(EditAnywhere, Category="Struct")
	FString StructName;
	
	inline static const FName StructPropertyName{"FragmentType"};
	
protected:

	UPROPERTY()
	UScriptStruct* Struct;
	
	inline static const FName OwnerPropertyName{"Owner"};
	inline static const FName HasResultPropertyName{"HasResult"};
	inline static const FName OutPropertyName{"Out"};
	inline static const FName IndexPropertyName{"Index"};
	inline static const FName MassFragmentPropertyName{"MassFragment"};
};
