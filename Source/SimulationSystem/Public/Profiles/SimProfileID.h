#pragma once

#include "CoreMinimal.h"
#include "SimProfileID.generated.h"

class USimProfileBase;

USTRUCT(BlueprintType)
struct SIMULATIONSYSTEM_API FReplicatedSimProfileID
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<USimProfileBase> Class;

	UPROPERTY()
	int ProfileID;

	FReplicatedSimProfileID(TSubclassOf<USimProfileBase> NewClass, uint32 NewProfileID): Class(NewClass), ProfileID(NewProfileID) {}

	FReplicatedSimProfileID(): Class(nullptr), ProfileID(-1) {}

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct SIMULATIONSYSTEM_API FSimProfileID
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<USimProfileBase> Class;

	UPROPERTY(VisibleAnywhere)
	uint32 ProfileID;

	FSimProfileID(TSubclassOf<USimProfileBase> NewClass, uint32 NewProfileID): Class(NewClass), ProfileID(NewProfileID) {}

	FSimProfileID(): Class(nullptr), ProfileID(static_cast<uint32>(-1)) {}

	bool IsValid() const;

	FString ToString() const;
};

bool SIMULATIONSYSTEM_API operator==(const FSimProfileID& A, const FSimProfileID& B);
bool SIMULATIONSYSTEM_API operator!=(const FSimProfileID& A, const FSimProfileID& B);

FORCEINLINE uint32 GetTypeHash(const FSimProfileID& Data)
{
	uint32 Hash = FCrc::MemCrc32(&Data, sizeof(Data));
	return Hash;
}