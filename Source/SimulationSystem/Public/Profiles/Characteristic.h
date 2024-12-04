#pragma once

#include "CoreMinimal.h"
#include "Characteristic.generated.h"

USTRUCT(BlueprintType)
struct FCharacteristic{ 
    
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int CurrentLevel;
    
    
};