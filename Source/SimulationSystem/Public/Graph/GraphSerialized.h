#pragma once

#include "CoreMinimal.h"
#include "SimEdgeStruct.h"
#include "SimulationArchetypeHandle.h"
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
struct FGraphLayerSerialized
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TMap<FSimVertexID, FVector> Vertices;

	UPROPERTY(VisibleAnywhere)
	TArray<FSimEdgeStruct> Edges;
	
};

USTRUCT(BlueprintType)
struct FGraphSerialized
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FGraphLayerSerialized> Layers;
	
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
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite)
	FSimulationArchetypeHandle Archetype;

	UPROPERTY(BlueprintReadWrite)
	FSimVertexID VertexLocation = FSimVertexID::Invalid;
	
	UPROPERTY(BlueprintReadWrite)
	bool HasData = false;

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
