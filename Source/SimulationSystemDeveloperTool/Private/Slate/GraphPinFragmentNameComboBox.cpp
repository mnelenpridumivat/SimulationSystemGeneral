#include "GraphPinFragmentNameComboBox.h"

#include "MassEntityTypes.h"

void SGraphPinFragmentNameComboBox::FillOptions()
{
	auto FragmentBaseStruct = FMassFragment::StaticStruct();

	for (TObjectIterator<UScriptStruct> It; It; ++It)
	{
		auto Struct = *It;
		if (Struct != FragmentBaseStruct
			&& Struct->IsChildOf(FragmentBaseStruct)
			&& Struct->HasMetaData(TEXT("BlueprintType")))
		{
			NameOptions.Add(MakeShared<FName>(Struct->GetName()));
		}
	}
}
