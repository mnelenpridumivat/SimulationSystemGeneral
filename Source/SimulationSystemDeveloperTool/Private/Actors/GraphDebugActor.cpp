// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphDebugActor.h"

#include "imgui.h"
//#include <imgui_internal.h>

#include "DebugData.h"
#include "DebugFragment.h"
#include "DebugProcessor.h"
#include "DrawDebugHelpers.h"
#include "GlobalGraph.h"
#include "GraphAsset.h"
#include "GraphPointBase.h"
#include "MassEntitySubsystem.h"
#include "MassExecutionContext.h"
#include "NavHeuristic_DistToDest.h"
#include "SimProfileCamp.h"
#include "SimProfileStash.h"
#include "SimulationArchetype.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationState.h"
#include "SimulationSystemDeveloperFunctionLibrary.h"
#include "SimulationSystemProfileType.h"
#include "SquadsDebugDrawProcessor.h"
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
	SaveName.Reset(256);
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
	switch(GetSimulationSystemProfileType())
	{
	case ESimualtionSystemProfileType::Classes:
		{
			BeginPlayClassesSetup();
			break;
		}
	case ESimualtionSystemProfileType::ECS:
		{
			BeginPlayECSSetup();
			break;
		}
	default:
		{
			ensure(false);
		}
	}

	if (IsValid(TestNavGraphPointA) && IsValid(TestNavGraphPointB))
	{
		TestWay = USimulationFunctionLibrary::FindWay(GetWorld(), TestNavGraphPointA->GetVertexID(), TestNavGraphPointB->GetVertexID(), {NewObject<UNavHeuristic_DistToDest>()});
	}
}

void AGraphDebugActor::BeginPlayClassesSetup()
{
	SelectedProfileClass = USimProfileBase::StaticClass();
	TSet<UClass*> AvalableClasses;
	for(TObjectIterator<UClass> It; It; ++It)
	{
		if(It->IsChildOf(USimProfileBase::StaticClass()))
		{
			AvalableClasses.Add(*It);
		}
	}
	AvailableClassArr = AvalableClasses.Array();
	AvailableClassArr.Sort([&](const UClass& A, const UClass& B){return A.GetName().Compare(B.GetName()) < 0;});
}

void AGraphDebugActor::BeginPlayECSSetup()
{
	SelectedArchetypeClass = USimulationArchetype::StaticClass();
 
	/*FARFilter Filter;
	Filter.Classes.Add(FilterClass.Get());
 
	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssets(Filter, AssetDataList);*/
}

void AGraphDebugActor::ImGuiStats_Classes()
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	ImGui::Text("Show stats for profile class: ");
	ImGui::SameLine();
	if(ImGui::BeginCombo("Profile classes", TCHAR_TO_ANSI(*SelectedProfileClass->GetName()))){
		for(auto& elem : AvailableClassArr)
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

void AGraphDebugActor::ImGuiStats_ECS()
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	ImGui::Text("Show stats for profile type: ");
	ImGui::SameLine();
	if(ImGui::BeginCombo("Profile classes", TCHAR_TO_ANSI(*SelectedArchetypeClass->GetName()))){
		for(auto& elem : AvailableClassArr)
		{
			bool IsSelected = elem == SelectedArchetypeClass;
			if(ImGui::Selectable(TCHAR_TO_ANSI(*elem->GetName()), elem == SelectedArchetypeClass))
			{
				SelectedArchetypeClass = elem;
			}
			if(IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	FScopeLock g(&FEntitiesForDebugContainer::EntitiesCS);
	auto& EntitiesToDraw = FEntitiesForDebugContainer::Entities;
	
	FStringFormatNamedArguments args;
	args.Add("ProfilesNum", EntitiesToDraw.Num());
	FString TotalProfiles = FString::Format(TEXT("Total profiles: {ProfilesNum}"), args);
	ImGui::Text(TCHAR_TO_ANSI(*TotalProfiles));
	if(ImGui::Button("Show profiles"))
	{
		ShowProfiles = !ShowProfiles;
	}

	ImGuiProfilesList(EntitiesToDraw);
}

void AGraphDebugActor::ImGuiStats()
{
	switch (GetSimulationSystemProfileType())
	{
	case ESimualtionSystemProfileType::Classes:
		{
			ImGuiStats_Classes();
			break;
		}
	case ESimualtionSystemProfileType::ECS:
		{
			ImGuiStats_ECS();
			break;
		}
	default:
		{
			ensure(false);
		}
	}
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
		for(auto& elem : AvailableClassArr)
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
		if (!Profile->IsA(SelectedProfileClass))
		{
			continue;
		}
		FString ProfileName = Profile->GetName();
		if (ImGui::Button(TCHAR_TO_ANSI(*ProfileName)) || Profile == NewSelectedProfile)
		{
			ImGuiUpdateProfileToResearch(Profile);
			NewSelectedProfile = nullptr;
		}
	}
	if (IsValid(SelectedProfileClass))
	{
		ImGuiDoResearch();
	}
	ImGui::EndChild();
}

void AGraphDebugActor::ImGuiProfilesList(const TArray<FMassEntityHandle>& Entities)
{
	ImGui::BeginChild("Profiles");
	auto EntitySubsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	auto& Manager = EntitySubsystem->GetEntityManager();
	for(auto& Entity : Entities)
	{
		/*if (!Profile->IsA(SelectedProfileClass))
		{
			continue;
		}*/
		if (!ensure(Manager.IsEntityValid(Entity)))
		{
			continue;
		}
		auto& StorageFragment = Manager.GetFragmentDataChecked<FDebugFragment>(Entity);
		if (!ensure(!StorageFragment.Archetype.IsNull()))
		{
			continue;
		}
		FString ProfileName = StorageFragment.Archetype.LoadSynchronous()->GetName();
		ProfileName.Append(TEXT("_Ind"));
		ProfileName.AppendInt(Entity.Index);
		ProfileName.Append(TEXT("_SN"));
		ProfileName.AppendInt(Entity.SerialNumber);
		if (ImGui::Button(TCHAR_TO_ANSI(*ProfileName)) || Entity == NewSelectedEntity)
		{
			ImGuiUpdateProfileToResearch(Entity);
			NewSelectedEntity.Reset();
		}
	}
	if (SelectedEntity.IsValid())
	{
		ImGuiDoResearch();
	}
	ImGui::EndChild();
}

void AGraphDebugActor::ImGuiUpdateProfileToResearch(USimProfileBase* Profile)
{
	auto OldData = Data;
	ProfileToResearch = Profile;
	ProfileToResearch->GetDebugData(Data);
	ImGui::OpenPopup("Research");
}

void AGraphDebugActor::ImGuiUpdateProfileToResearch(FMassEntityHandle Entity)
{
	auto OldData = Data;
	EntityToResearch = Entity;
	//ProfileToResearch->GetDebugData(Data);
	ImGui::OpenPopup("Research");
}

void AGraphDebugActor::ImGuiDoResearch()
{
	if (ImGui::BeginPopup("Research"))
	{
		switch (GetSimulationSystemProfileType())
		{
		case ESimualtionSystemProfileType::Classes:
			{
				ImGui::Text(StringCast<ANSICHAR>(*ProfileToResearch->GetFName().ToString()).Get());
				ImGui::BeginTable("Research Table", 2);
				for (auto elem : Data.Elems)
				{
					ImGuiDoResearchRec(elem);
				}
				ImGui::EndTable();
				break;
			}
		case ESimualtionSystemProfileType::ECS:
			{
				break;
			}
		default:
			{
				ensure(false);
			}
		}
		ImGui::EndPopup();
	}
}

void AGraphDebugActor::ImGuiDoResearchRec(DebugDataElemBase* Elem, int Tabs)
{
	ImGui::TableNextRow();
	switch (Elem->GetType())
	{
	case EDebugDataType::KeyValue:
		{
			auto Casted = static_cast<DebugDataElemKeyValue*>(Elem);
			ImGui::TableSetColumnIndex(0);
			ImGui::TreeNodeEx(StringCast<ANSICHAR>(*Casted->key.ToString()).Get(), ImGuiTreeNodeFlags_Leaf);
			ImGui::TreePop();
			//ImGuiDoResearchDrawKey(Casted->key.ToString(), Tabs);
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(StringCast<ANSICHAR>(*Casted->value).Get());
			
			break;
		}
	case EDebugDataType::Nested:
		{
			auto Casted = static_cast<DebugDataElemNested*>(Elem);
			ImGui::TableSetColumnIndex(0);
			bool TreeOpened = ImGui::TreeNodeEx(StringCast<ANSICHAR>(*Casted->key.ToString()).Get());
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", Casted->values.Num());
			if (TreeOpened)
			{
				for (auto Child : Casted->values)
				{
					ImGuiDoResearchRec(Child, Tabs + 1);
				}
				ImGui::TreePop();
			}
			break;
		}
	case EDebugDataType::KeyObject:
		{
			auto Casted = static_cast<DebugDataElemKeyObject*>(Elem);
			ImGui::TableSetColumnIndex(0);
			ImGui::TreeNodeEx(StringCast<ANSICHAR>(*Casted->key.ToString()).Get(), ImGuiTreeNodeFlags_Leaf);
			ImGui::TreePop();
			//ImGuiDoResearchDrawKey(Casted->key.ToString(), Tabs);
			ImGui::TableSetColumnIndex(1);
			const ANSICHAR* AnsiName = nullptr;
			if (Casted->object)
			{
				AnsiName = StringCast<ANSICHAR>(*Casted->object->GetName()).Get();
			} else
			{
				AnsiName = "none";
			}
			if (auto Profile = Cast<USimProfileBase>(Casted->object); IsValid(Profile))
			{
				if (ImGui::Button(AnsiName))
				{
					NewSelectedProfile = Profile;
				}
			} else
			{
				ImGui::Text(AnsiName);
			}
			break;
		}
	default: ensure(false);
	}
}

void AGraphDebugActor::DrawSquadsDebugDraw()
{
	FScopeLock g(&FSquadsDebugDrawContainer::Lock);
	{
		// Graph positions
		auto& Points = FSquadsDebugDrawContainer::GraphPositions;
		for (auto& Point : Points)
		{
			DrawDebugSphere(GetWorld(), Point, 100.0f, 8, FColor(0, 128, 0), false, 0, 0, 10);
		}
	}
	{
		// Real positions
		auto& Points = FSquadsDebugDrawContainer::EntityPositions;
		for (auto& Point : Points)
		{
			DrawDebugSphere(GetWorld(), Point, 100.0f, 8, FColor(255, 0.0f, 0.0f), false, 0, 0, 10);
		}
	}
	{
		// Ways
		auto& Lines = FSquadsDebugDrawContainer::WayLines;
		for (auto& [A, B] : Lines)
		{
			DrawDebugLine(GetWorld(), A, B, FColor(64, 128, 255), false, 0, 0, 5);
		}
	}
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
		switch (GetSimulationSystemProfileType())
		{
		case ESimualtionSystemProfileType::Classes:
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
				break;
			}
		case ESimualtionSystemProfileType::ECS:
			{
				DrawSquadsDebugDraw();
				break;
			}
			default:
			{
				ensure(false);
			}
		}
	}

	if(ShowGraph){
		USimulationSystemDeveloperFunctionLibrary::DebugDrawGraph(GetWorld(), DeltaTime*1.1f);
	}

	ImGui::End();

	if (TestWay.IsValid())
	{
		auto& Way = TestWay.GetWay();
		for (size_t i = 1; i < Way.Num; ++i)
		{
			auto Prev = Way.Vertices[i-1].Pin();
			auto Cur = Way.Vertices[i].Pin();
			DrawDebugLine(GetWorld(), Prev->GetLocation(), Cur->GetLocation(), FColor::Red, false, 0, 0, 50.0f);
		}
	}
	
}

