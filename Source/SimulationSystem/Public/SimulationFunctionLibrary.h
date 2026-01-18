// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionPlannerOwner.h"
#include "GraphWay.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Profiles/SimProfileHolder.h"
#include "SimulationFunctionLibrary.generated.h"

class ULocalGraphRegistry;
class AGraphAsset;
class UProfileIDController;
struct FSimProfileID;
struct FBytesSerialization;
class USimulationSystemSubsystem;
class USimulationSystemFunctionsImplementation;
class UAISimProfileSquad;
class UNavHeuristic_Base;
class USimProfileBase;
class AGlobalGraph;

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static const TCHAR* GetWorldTcharNameChecked(UObject* Context);

public:

	UFUNCTION(BlueprintPure, meta=(WorldContext="Context"))
	static AGlobalGraph* GetGlobalGraph(UObject* Context);

	UFUNCTION(BlueprintPure, meta=(WorldContext="Context"))
	static USimProfileBase* GetProfile(UObject* Context, const FSimProfileID& ProfileID);

	UFUNCTION(BlueprintPure, meta=(WorldContext="Context"))
	static AGraphAsset* GetGraphAsset(UObject* Context, const FSimVertexID& VertexInsideID);
	static AGraphAsset* GetGraphAsset(UObject* Context, int ChunkIndex);

	UFUNCTION(BlueprintPure, meta=(WorldContext="Context"))
	static AGraphAsset* GetGraphAssetFromProfile(UObject* Context, USimProfileBase* Profile);

	UFUNCTION(BlueprintPure, meta=(WorldContext="Context"))
	static ULocalGraphRegistry* GetLocalGraphRegistry(UObject* Context, const FSimVertexID& VertexInsideID);
	static ULocalGraphRegistry* GetLocalGraphRegistry(UObject* Context, int ChunkIndex);

	template<typename T>
	static T* GetProfileCasted(UObject* Context, const FSimProfileID& ProfileID)
	{
		return Cast<T>(GetProfile(Context, ProfileID));
	}

	UFUNCTION(BlueprintPure, meta=(WorldContext="Context"))
	static UProfileIDController* GetProfileIDController(UObject* Context);

	UFUNCTION(BlueprintPure)
	static void AsProfile(const FSimProfileHolder& Holder, USimProfileBase*& Profile, bool& Success);

	UFUNCTION(BlueprintPure)
	static void AsVertex(const FSimProfileHolder& Holder, FSimVertexID& VertexID, bool& Success);

	UFUNCTION(BlueprintPure)
	static FSimVertexID GetWayStart(const FGraphWay& Way);

	UFUNCTION(BlueprintPure)
	static FSimVertexID GetWayEnd(const FGraphWay& Way);

	UFUNCTION(BlueprintPure)
	static FSimVertexID GetWayCurrentGraphPoint(const FGraphWay& Way);

	UFUNCTION(BlueprintPure)
	static FSimVertexID GetWayNextGraphPoint(const FGraphWay& Way);

	UFUNCTION(BlueprintPure, meta=(WorldContext=Context))
	static FGraphWay FindWay(UObject* Context, const FSimVertexID& Start, const FSimVertexID& Dest, TSet<UNavHeuristic_Base*> Params);

	static USimulationSystemSubsystem* GetSimulationSystemSubsystem(UObject* Context);
	
	UFUNCTION(BlueprintPure, meta=(WorldContext=Context))
	static USimulationSystemFunctionsImplementation* GetFunctions(UObject* Context);

	static void SaveObjectData(UObject* Object, FBytesSerialization& Data);
	static void LoadObjectData(UObject* Object, const FBytesSerialization& Data);

	// UK2Nodes
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	static UScriptStruct* GetStructFromName(const FString& StructName);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, meta=(HidePin="Out"))
	static bool GetConstFragmentData(UActionPlannerOwner* Owner, UScriptStruct* Struct, FFragmentProperties& Out);
	
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, meta=(HidePin="Prop"))
	static int GetIntFragmentProperty(const FFragmentProperties& MassFragment, int Index);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	static float GetFloatFragmentProperty(const FFragmentProperties& MassFragment, int Index);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	static double GetDoubleFragmentProperty(const FFragmentProperties& MassFragment, int Index);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	static bool GetBoolFragmentProperty(const FFragmentProperties& MassFragment, int Index);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	static FString GetStringFragmentProperty(const FFragmentProperties& MassFragment, int Index);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	static FName GetNameFragmentProperty(const FFragmentProperties& MassFragment, int Index);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	static FText GetTextFragmentProperty(const FFragmentProperties& MassFragment, int Index);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	static UObject* GetObjectFragmentProperty(const FFragmentProperties& MassFragment, int Index);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	static UClass* GetClassFragmentProperty(const FFragmentProperties& MassFragment, int Index);
	
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	static void GetStructFragmentProperty(const FFragmentProperties& MassFragment, int Index, FInstancedStruct& Out);
	
};
