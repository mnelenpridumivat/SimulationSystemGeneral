#pragma once

#include "CoreMinimal.h"
#include "SimEdgeStruct.h"
#include "SimVertexID.h"
#include "GraphSerialized.generated.h"

class USimProfileBase;

USTRUCT(BlueprintType)
struct FBytesSerialization
{

	GENERATED_BODY()

	UPROPERTY()
	TArray<uint8> SerializedObject;
	
};

USTRUCT()
struct FGraphSerialized
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FSimVertexID, FVector> Vertices;

	UPROPERTY()
	TArray<FSimEdgeStruct> Edges;
	
};

struct FProfilesSerialized;
struct FProfilesSerializedView;
struct FSerializedProfile;

/*
 * Encapsulate element like ref, safe during parent reallocation
 */
USTRUCT(BlueprintType)
struct SIMULATIONSYSTEM_API FSerializedProfileView
{
	GENERATED_BODY()

	FProfilesSerialized* GlobalParent;
	int Index;

	FSerializedProfile& GetElem();
	
};

USTRUCT()
struct SIMULATIONSYSTEM_API FSerializedProfileChildrenChunk
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int> Children = {};
	
};

USTRUCT(BlueprintType)
struct SIMULATIONSYSTEM_API FSerializedProfile
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FSimVertexID VertexLocation = FSimVertexID::Invalid;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UObject> ObjectClass;

	UPROPERTY(BlueprintReadWrite)
	FBytesSerialization ObjectData;

	UPROPERTY()
	TArray<FSerializedProfileChildrenChunk> Children = {};

	FProfilesSerialized* GlobalParent;
	FSerializedProfile* Parent = nullptr;

	void ExtractFirstChildren(FProfilesSerializedView& ExtractedChildren);
	FSerializedProfileView AddChild();
	void NextSet();
	
};

USTRUCT(BlueprintType)
struct SIMULATIONSYSTEM_API FProfilesSerializedView
{
	GENERATED_BODY()

	TArray<FSerializedProfile*> Objects;
	
};

USTRUCT(BlueprintType)
struct SIMULATIONSYSTEM_API FProfilesSerialized
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FSerializedProfile> Objects;

	FSerializedProfileView AddLast();
	
};
