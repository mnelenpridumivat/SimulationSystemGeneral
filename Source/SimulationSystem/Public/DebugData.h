#pragma once

#include "CoreMinimal.h"
#include "Containers/Union.h"

#include "DebugData.generated.h"

UENUM(BlueprintType)
enum class EDebugDataType : uint8
{
	Null,
	KeyValue,
	Nested,
	KeyObject,
};

struct SIMULATIONSYSTEM_API DebugDataElemBase
{
	virtual ~DebugDataElemBase() = default;
	virtual EDebugDataType GetType();
};

struct SIMULATIONSYSTEM_API DebugDataElemKeyValue: DebugDataElemBase
{
	FName key;
	FString value;

	virtual EDebugDataType GetType() override;
};

struct SIMULATIONSYSTEM_API DebugDataElemKeyObject: DebugDataElemBase
{
	FName key;
	UObject* object = nullptr;

	virtual EDebugDataType GetType() override;
};

struct SIMULATIONSYSTEM_API DebugDataElemNested: DebugDataElemBase
{
	FName key;
	TArray<DebugDataElemBase*> values;
	bool Collapsed = false;

	~DebugDataElemNested();

	virtual EDebugDataType GetType() override;
};

USTRUCT()
struct SIMULATIONSYSTEM_API FDebugDataMain
{
	GENERATED_BODY()

	~FDebugDataMain();

	TArray<DebugDataElemBase*> Elems;
	
};

struct DebugDataPropertyHandle
{
	FName Name;
	FProperty* Type;
	void* Context;

	FName GetName();
};
