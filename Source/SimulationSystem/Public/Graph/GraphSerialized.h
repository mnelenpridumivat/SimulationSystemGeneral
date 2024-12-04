#pragma once

#include "CoreMinimal.h"
#include "SimEdgeStruct.h"
#include "SimVertexID.h"
#include "GraphSerialized.generated.h"

class USimProfileBase;

USTRUCT()
struct FProfileSerialization
{

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<uint8> SerializedObject;

	/*UFUNCTION(BlueprintCallable)
	UObject* DeserializeObject();

	UFUNCTION(BlueprintCallable)
	void SerializeObject(UObject* Object);*/

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSoftObjectPtr<UObject> Class;
	//FArchive Serialized;
	
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

USTRUCT()
struct SIMULATIONSYSTEM_API FSerializedProfile
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 ChildrenNum = 0;

	UPROPERTY()
	FSimVertexID VertexLocation = FSimVertexID::Invalid;

	UPROPERTY()
	TSubclassOf<UObject> ObjectClass;

	UPROPERTY()
	FProfileSerialization ObjectData;

	//UPROPERTY()
	//USimProfileBase* Object;
	
};

USTRUCT()
struct FProfilesSerialized
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FSerializedProfile> Objects;
	
};
