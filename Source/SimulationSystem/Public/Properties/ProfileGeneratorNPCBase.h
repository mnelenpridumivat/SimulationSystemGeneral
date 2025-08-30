#pragma once

#include "CoreMinimal.h"
#include "ProfileGeneratorNPCBase.generated.h"

USTRUCT(BlueprintType)
struct FNPCDataBase : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Class;
};