// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphDebugActor.h"

#include "imgui.h"
//#include <imgui_internal.h>

#include "DrawDebugHelpers.h"
#include "GlobalGraph.h"
#include "GraphAsset.h"
#include "SimProfileCamp.h"
#include "SimProfileStash.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationState.h"
#include "SimulationSystemDeveloperFunctionLibrary.h"
#include "SquadTask_MoveToCamp.h"
#include "Vertex.h"
#include "Kismet/GameplayStatics.h"
#include "TaskDebug/TaskDebugMoveToCamp.h"


// Sets default values
AGraphDebugActor::AGraphDebugActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	buf = new char[256];
	TaskDebuggersClasses.Add(USquadTask_MoveToCamp::StaticClass(), UTaskDebugMoveToCamp::StaticClass());
	//other task debuggers here
	TaskDebuggersClasses.Add(USquadTaskBase::StaticClass(), UTaskDebugBase::StaticClass());
}

AGraphDebugActor::~AGraphDebugActor()
{
	delete[] buf;
}

// Called when the game starts or when spawned
void AGraphDebugActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);
	SelectedProfileClass = USimProfileBase::StaticClass();
	SaveName.Reset(256);
	TSet<UClass*> AvalableClasses;
	for(TObjectIterator<UClass> It; It; ++It)
	{
		if(It->IsChildOf(USimProfileBase::StaticClass()))
		{
			AvalableClasses.Add(*It);
		}
	}
	AvalableClassArr = AvalableClasses.Array();
	AvalableClassArr.Sort([&](const UClass& A, const UClass& B){return A.GetName().Compare(B.GetName()) < 0;});
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	auto& Chunks = GlobalGraph->GetChunks();
	if(Chunks.Num())
	{
		SelectedChunk = Chunks[0];
	}
	for(auto& elem : TaskDebuggersClasses)
	{
		TaskDebuggers.Add(elem.Key, NewObject<UTaskDebugBase>(GetWorld(), elem.Value));
	}
	/*FTimerHandle hand;
	GetWorldTimerManager().SetTimer(hand, [&]()
	{
		SetActorTickEnabled(true);
	}, 5.0f, false);*/
	
}

void AGraphDebugActor::ImGuiStats()
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	ImGui::Text("Show stats for profile class: ");
	ImGui::SameLine();
	if(ImGui::BeginCombo("Profile classes", TCHAR_TO_ANSI(*USimProfileBase::StaticClass()->GetName()))){
		for(auto& elem : AvalableClassArr)
		{
			bool IsSelected = elem == SelectedProfileClass;
			if(ImGui::Selectable(TCHAR_TO_ANSI(*elem->GetName()), elem == SelectedProfileClass))
			{
				SelectedProfileClass = elem;
			}
			if(IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	TArray<USimProfileBase*> Profiles;
	GlobalGraph->GetProfiles(Profiles);
	int NumOfSelectedClass = 0;
	for(auto& elem : Profiles)
	{
		if(elem->IsA(SelectedProfileClass))
		{
			++NumOfSelectedClass;
		}
	}
	FStringFormatNamedArguments args;
	args.Add("ProfilesNum", NumOfSelectedClass);
	FString TotalProfiles = FString::Format(TEXT("Total profiles: {ProfilesNum}"), args);
	ImGui::Text(TCHAR_TO_ANSI(*TotalProfiles));
	if(ImGui::Button("Show profiles"))
	{
		ShowProfiles = !ShowProfiles;
	}
	Profiles.Sort([&](const USimProfileBase& A, const USimProfileBase& B){return A.GetName().Compare(B.GetName()) < 0;});
	ImGuiProfilesList(Profiles);
}

void AGraphDebugActor::ImGuiGraph()
{
	if(ImGui::Button("Show graph"))
	{
		ShowGraph = !ShowGraph;
	}
}

void AGraphDebugActor::ImGuiWorld()
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	if(ImGui::CollapsingHeader("Save and load world"))
	{
		ImGui::Text("Save name: ");
		ImGui::SameLine();
		ImGui::InputText("text", buf, 256);
		SaveName = FString(buf);
		if(ImGui::Button("Save world"))
		{
			auto Save = NewObject<USimulationState>(GetWorld());
			GlobalGraph->SaveObjects(Save);
			UGameplayStatics::SaveGameToSlot(Save, SaveName, 0);
		}
		if(ImGui::Button("Load world"))
		{
			auto Save = Cast<USimulationState>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
			Save->Rename(nullptr, GetWorld());
			if(Save)
			{
				GlobalGraph->UnloadObjects();
				GlobalGraph->LoadObjects_Save(Save);
			}
		}
		if(ImGui::Button("Reset"))
		{
			GlobalGraph->UnloadObjects();
			GlobalGraph->LoadObjects_Initial();
		}
	}
	/*if(ImGui::CollapsingHeader("Chunks"))
	{
		auto& Chunks = GlobalGraph->GetChunks();
		FStringFormatNamedArguments args;
		args.Add("ChunksNum", Chunks.Num());
		FString TotalProfiles = FString::Format(TEXT("Total Chunks: {ChunksNum}"), args);
		ImGui::Text(TCHAR_TO_ANSI(*TotalProfiles));
		if(Chunks.Num())
		{
			ImGui::SameLine();
			if(ImGui::BeginCombo("Chunks", TCHAR_TO_ANSI(*SelectedChunk->GetName())))
			{
				for(auto& Chunk : Chunks)
				{
					bool IsSelected = Chunk == SelectedChunk;
					if(ImGui::Selectable(TCHAR_TO_ANSI(*Chunk->GetName()), Chunk == SelectedChunk))
					{
						SelectedChunk = Chunk;
					}
					if(IsSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
	}*/
}

void AGraphDebugActor::ImGuiChunks()
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	auto& Chunks = GlobalGraph->GetChunks();
	FStringFormatNamedArguments args;
	args.Add("ChunksNum", Chunks.Num());
	FString TotalProfiles = FString::Format(TEXT("Total Chunks: {ChunksNum}"), args);
	ImGui::Text(TCHAR_TO_ANSI(*TotalProfiles));
	if(Chunks.Num())
	{
		if(ImGui::BeginCombo("Chunks", TCHAR_TO_ANSI(*SelectedChunk->GetName())))
		{
			for(auto& Chunk : Chunks)
			{
				bool IsSelected = Chunk == SelectedChunk;
				if(ImGui::Selectable(TCHAR_TO_ANSI(*Chunk->GetName()), Chunk == SelectedChunk))
				{
					SelectedChunk = Chunk;
				}
				if(IsSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	if(!SelectedChunk)
	{
		return;
	}
	TArray<USimProfileBase*> ProfilesInChunk;
	GlobalGraph->GetProfilesInChunk(SelectedChunk->GetChunkIndex(), ProfilesInChunk);
	ProfilesInChunk = ProfilesInChunk.FilterByPredicate([&](const USimProfileBase* A){return A->IsA(SelectedProfileClass);});
	args.Empty();
	args.Add("ProfilesNum", ProfilesInChunk.Num());
	TotalProfiles = FString::Format(TEXT("Total profiles in chunk: {ProfilesNum}"), args);
	ImGui::Text(TCHAR_TO_ANSI(*TotalProfiles));
	ImGui::SameLine();
	if(ImGui::BeginCombo("Profile classes", TCHAR_TO_ANSI(*USimProfileBase::StaticClass()->GetName()))){
		for(auto& elem : AvalableClassArr)
		{
			bool IsSelected = elem == SelectedProfileClass;
			if(ImGui::Selectable(TCHAR_TO_ANSI(*elem->GetName()), elem == SelectedProfileClass))
			{
				SelectedProfileClass = elem;
			}
			if(IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ProfilesInChunk.Sort([&](const USimProfileBase& A, const USimProfileBase& B){return A.GetName().Compare(B.GetName()) < 0;});
	ImGuiProfilesList(ProfilesInChunk);
}

void AGraphDebugActor::ImGuiProfilesList(const TArray<USimProfileBase*>& Profiles)
{
	// TODO: Fix tooltip and scrollbar
	ImGui::BeginChild("Profiles");
	for(auto& Profile : Profiles)
	{
		FString TotalProfiles = Profile->GetName();
		ImGui::Text(TCHAR_TO_ANSI(*TotalProfiles));
		ProfileToResearch = nullptr;
        if (ImGui::IsItemHovered())
        {
        	ImGui::BeginTooltip();
        	if(ImGui::Button("More...")){
        		ProfileToResearch = Profile;
        	}
        	ImGui::EndTooltip();
        }
	}
	ImGui::EndChild();
}

// Called every frame
void AGraphDebugActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ImGui::Begin("Simulation system debug tool", nullptr, ImGuiWindowFlags_MenuBar);
	
	ImGui::SetWindowCollapsed(true, ImGuiCond_Appearing);

	ImGui::SetWindowSize({ImGui::GetMainViewport()->Size.x/3, ImGui::GetMainViewport()->Size.y/3}, ImGuiCond_Appearing);
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::MenuItem("Stats"))
		{
			SelectedDebugWindow = EDebugWindow_Stats;
		}
		if(ImGui::MenuItem("Graph"))
		{
			SelectedDebugWindow = EDebugWindow_Graph;
		}
		if(ImGui::MenuItem("World"))
		{
			SelectedDebugWindow = EDebugWindow_World;
		}
		if(ImGui::MenuItem("Chunks"))
		{
			SelectedDebugWindow = EDebugWindow_Chunks;
		}
	}
	ImGui::EndMenuBar();

	switch (SelectedDebugWindow)
	{
	case EDebugWindow_Stats:
		{
			ImGuiStats();
			break;
		}
	case EDebugWindow_Graph:
		{
			ImGuiGraph();
			break;
		}
	case EDebugWindow_World:
		{
			ImGuiWorld();
			break;
		}
	case EDebugWindow_Chunks:
		{
			ImGuiChunks();
			break;
		}
	}
	
	if(ShowProfiles)
	{
		auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
		TArray<USimProfileBase*> Profiles;
		GlobalGraph->GetProfiles(Profiles);
		for(auto& elem : Profiles)
		{
			if(!elem->IsA(SelectedProfileClass))
			{
				continue;
			}
			FVector Location = GlobalGraph->GetVertexByID(GlobalGraph->GetProfileLocationOnGraph(elem)).Pin()->GetLocation();
			if(elem->IsA(USimProfileCamp::StaticClass()))
			{
				DrawDebugSphere(GetWorld(), Location, 250.0f, 8, FColor(0.209f, 0.0f, 1.0f), false, DeltaTime*1.1f, 0, 10);
			} else if (elem->IsA(USimProfileStash::StaticClass()))
			{
				DrawDebugBox(GetWorld(), Location, FVector{100.0f}, FColor::White, false, DeltaTime*1.1f, 0, 10);
			} else if (elem->IsA(UAISimProfileSquad::StaticClass()))
			{
				DrawDebugCapsule(GetWorld(), Location, 200.0f, 100.0f, FRotator::ZeroRotator.Quaternion(), FColor::Green, false, DeltaTime*1.1f, 0, 10);
				auto Casted = Cast<UAISimProfileSquad>(elem);
					if(Casted->GetCurrentTask())
					{
						for(auto& Debugger : TaskDebuggers)
						{
							if(Casted->GetCurrentTask()->IsA(Debugger.Key))
							{
								Debugger.Value->VisualizeTaskInfo(DeltaTime, Casted, Casted->GetCurrentTask());
								break;
							}
						}
					}
			}
		}
	}

	if(ShowGraph){
		USimulationSystemDeveloperFunctionLibrary::DebugDrawGraph(GetWorld(), DeltaTime*1.1f);
	}

	ImGui::End();
	
}

