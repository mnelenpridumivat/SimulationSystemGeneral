// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphDebugActor.generated.h"

class UTaskDebugBase;
class USquadTaskBase;
class USimProfileBase;
class AGraphAsset;

UCLASS()
class SIMULATIONSYSTEMDEVELOPERTOOL_API AGraphDebugActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGraphDebugActor();
	~AGraphDebugActor();

protected:

	enum DebugWindows{
		EDebugWindow_Stats,
		EDebugWindow_Graph,
		EDebugWindow_World,
		EDebugWindow_Chunks
	};
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool DebugCollapsed = true;
	DebugWindows SelectedDebugWindow = EDebugWindow_Stats;
	bool ShowProfiles = false;
	bool ShowGraph = false;
	char* buf;
	FString SaveName;

	UPROPERTY()
	UClass* SelectedProfileClass = nullptr;

	UPROPERTY()
	AGraphAsset* SelectedChunk = nullptr;

	UPROPERTY()
	USimProfileBase* ProfileToResearch = nullptr;

	UPROPERTY()
	TArray<UClass*> AvalableClassArr;

	UPROPERTY()
	TMap<TSubclassOf<USquadTaskBase>, TSubclassOf<UTaskDebugBase>> TaskDebuggersClasses;

	UPROPERTY()
	TMap<TSubclassOf<USquadTaskBase>, UTaskDebugBase*> TaskDebuggers;

	void ImGuiStats();
	void ImGuiGraph();
	void ImGuiWorld();
	void ImGuiChunks();

	void ImGuiProfilesList(const TArray<USimProfileBase*>& Profiles);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
