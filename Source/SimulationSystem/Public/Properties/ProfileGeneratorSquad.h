#pragma once

#include "CoreMinimal.h"
#include "ProfileGenerator.h"
#include "ProfileGeneratorSquad.generated.h"

USTRUCT(BlueprintType)
struct FSquadNPCData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Name;
};

USTRUCT(BlueprintType)
struct FSquadDataHandle
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Name;
};

USTRUCT(BlueprintType)
struct FSquadData : public FTableRowBase{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSquadNPCData> NPCs = {};
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool Random = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Random", ClampMin = "1", UIMin = "1"))
    int Min = 1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Random", ClampMin = "1", UIMin = "1"))
    int Max = 1;
};

USTRUCT(BlueprintType)
struct SIMULATIONSYSTEM_API FGeneratorHandleSquad : public FGeneratorHandleBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSquadDataHandle SquadName;
    
	virtual void SetupGenerator(UProfileGenerator* Generator) const override;
    
};

UCLASS()
class SIMULATIONSYSTEM_API UProfileGeneratorSquad : public UProfileGenerator
{
    GENERATED_BODY()

public:

    virtual USimProfileBase* GenerateProfile() override;

    void SetSquadName(FName NewSquadName){ SquadName = NewSquadName; }
    
protected:

    UPROPERTY()
    FName SquadName;
    
};