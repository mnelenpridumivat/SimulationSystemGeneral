#pragma once

#include "CoreMinimal.h"
#include "SimVertexID.generated.h"

USTRUCT(BlueprintType)
struct SIMULATIONSYSTEM_API FSimVertexID
{
	GENERATED_BODY()

	UPROPERTY()
	uint16 ChunkID;

	UPROPERTY()
	uint8 LevelID;

	UPROPERTY()
	uint16 VertexID;

	bool IsValid() const
	{
		return ChunkID!=static_cast<uint16>(-1)
		&& LevelID!=static_cast<uint8>(-1)
		&& VertexID!=static_cast<uint16>(-1);
	}

	static const FSimVertexID Invalid;
	
};

inline bool operator==(const FSimVertexID& A, const FSimVertexID& B)
{
	return A.ChunkID == B.ChunkID && A.LevelID == B.LevelID && A.VertexID == B.VertexID;
}

//inline bool operator==(FSimVertexID&& A, FSimVertexID&& B)
//{
//	return A.ChunkID == B.ChunkID && A.LevelID == B.LevelID && A.VertexID == B.VertexID;
//}

inline bool operator!=(const FSimVertexID& A, const FSimVertexID& B)
{
	return !(A==B);
}

//inline bool operator!=(FSimVertexID&& A, FSimVertexID&& B)
//{
//	return A!=B;
//}

FORCEINLINE uint32 GetTypeHash(const FSimVertexID& Data)
{
	uint32 Hash = FCrc::MemCrc32(&Data, sizeof(Data));
	return Hash;
}

FORCEINLINE FArchive &operator <<(FArchive &Ar, FSimVertexID& ID )
{
	Ar << ID.ChunkID;
	Ar << ID.LevelID;
	Ar << ID.VertexID;
		
	return Ar;
}