// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\SimulationFunctionLibrary.h"

#include "EngineUtils.h"
#include "GlobalGraph.h"
#include "GraphAsset.h"
#include "GraphSerialized.h"
#include "SimulationSystemSettings.h"
#include "NativePriorityQueue.h"
#include "ProfileIDController.h"
#include "SimulationSystemFunctionsImplementation.h"
#include "SimulationSystemSubsystem.h"
#include "Vertex.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Vertex.h"
#include "NavHeuristics/NavHeuristic_Base.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "ActionPlannerOwner.h"

const TCHAR* USimulationFunctionLibrary::GetWorldTcharNameChecked(UObject* Context)
{
	return *(Context ? (Context->GetWorld() ? Context->GetWorld()->GetName() : FString("null")) : FString(
				   "null (context)"));
}

AGlobalGraph* USimulationFunctionLibrary::GetGlobalGraph(UObject* Context)
{
#if WITH_EDITOR
	if (GIsEditor && Context->GetWorld()->WorldType != EWorldType::Editor)
#endif
	{
		if (!ensureMsgf(
			IsValid(Context),
			TEXT("Attempt to call GetGlobalGraph with invalid Context!")))
		{
			return nullptr;
		}
		auto Subsystem = GetSimulationSystemSubsystem(Context);
		if (!ensure(IsValid(Subsystem)))
		{
			return nullptr;
		}
		return Subsystem->GetGlobalGraph();
	}
#if WITH_EDITOR
	// in editor time SimulationSystemSubsystem has no GlobalGraph - search across all objects
	TArray<AActor*> Graphs;
	UGameplayStatics::GetAllActorsOfClass(Context, AGlobalGraph::StaticClass(), Graphs);
	if (!ensureMsgf(Graphs.Num(), TEXT("Unable to find any global graphs!")))
	{
		return nullptr;
	}
	ensureMsgf(!(Graphs.Num()-1), TEXT("There is more that one global graphs!"));
	return Cast<AGlobalGraph>(Graphs[0]);
	
#endif
}

USimProfileBase* USimulationFunctionLibrary::GetProfile(UObject* Context, const FSimProfileID& ProfileID)
{
	auto ProfileIDControllerPtr = GetProfileIDController(Context);
	if (!ensure(IsValid(ProfileIDControllerPtr)))
	{
		return nullptr;
	}
	return ProfileIDControllerPtr->GetProfile(ProfileID);
}

AGraphAsset* USimulationFunctionLibrary::GetGraphAsset(UObject* Context, const FSimVertexID& VertexInsideID)
{
	if (!ensureMsgf(
		VertexInsideID.IsValid(),
		TEXT("Call GetGraphAsset with invalid VertexID!")))
	{
		return nullptr;
	}
	return GetGraphAsset(Context, VertexInsideID.ChunkID);
}

AGraphAsset* USimulationFunctionLibrary::GetGraphAsset(UObject* Context, int ChunkIndex)
{
	if (!ensureMsgf(
			IsValid(Context),
			TEXT("Call GetGraphAsset without Context!"))
			|| !ChunkIndex)
	{
		return nullptr;
	}
	auto RealIndex = ChunkIndex - 1;
	auto GlobalGraph = GetGlobalGraph(Context);
	if(!ensureMsgf(
		IsValid(GlobalGraph),
		TEXT("GlobalGraph not found!")))
	{
		return nullptr;
	}
	return GlobalGraph->GetChunkByID(RealIndex);
}

AGraphAsset* USimulationFunctionLibrary::GetGraphAssetFromProfile(UObject* Context, USimProfileBase* Profile)
{
	if (!ensureMsgf(
			IsValid(Context),
			TEXT("Call GetGraphAsset without Context!")))
	{
		return nullptr;
	}
	auto GlobalGraph = GetGlobalGraph(Context);
	if(!ensureMsgf(
		IsValid(GlobalGraph),
		TEXT("GlobalGraph not found!")))
	{
		return nullptr;
	}
	auto ProfileLocation = GlobalGraph->GetProfileLocationOnGraph(Profile);
	if(!ensureMsgf(
		ProfileLocation.IsValid(),
		TEXT("Profile [%s] has invalid location on graph!"),
		*Profile->GetName()))
	{
		return nullptr;
	}
	return GlobalGraph->GetChunkByID(ProfileLocation);
}

ULocalGraphRegistry* USimulationFunctionLibrary::GetLocalGraphRegistry(UObject* Context,
                                                                       const FSimVertexID& VertexInsideID)
{
	if (!ensureMsgf(
		VertexInsideID.IsValid(),
		TEXT("Call GetLocalGraphRegistry with invalid VertexID!")))
	{
		return nullptr;
	}
	return GetLocalGraphRegistry(Context, VertexInsideID.ChunkID);
}

ULocalGraphRegistry* USimulationFunctionLibrary::GetLocalGraphRegistry(UObject* Context, int ChunkIndex)
{
	if (!ensureMsgf(
			IsValid(Context),
			TEXT("Call GetLocalGraphRegistry without Context!"))
			|| !ChunkIndex)
	{
		return nullptr;
	}
	return GetGraphAsset(Context, ChunkIndex)->GetRegistry();
}

UProfileIDController* USimulationFunctionLibrary::GetProfileIDController(UObject* Context)
{
	const auto GlobalGraph = GetGlobalGraph(Context);
	if (!ensureMsgf(IsValid(GlobalGraph), TEXT("Unable to access global graph actor in world [%s]"), GetWorldTcharNameChecked(Context)))
	{
		return nullptr;
	}
	return GlobalGraph->GetProfileIDsController();
}

void USimulationFunctionLibrary::AsProfile(const FSimProfileHolder& Holder, USimProfileBase*& Profile, bool& Success)
{
	Success = Holder.Holder.GetCurrentSubtypeIndex() == 1;
	if(Success)
	{
		Profile = Holder.Holder.GetSubtype<USimProfileBase*>();
	}
}

void USimulationFunctionLibrary::AsVertex(const FSimProfileHolder& Holder, FSimVertexID& VertexID, bool& Success)
{
	Success = Holder.Holder.GetCurrentSubtypeIndex() == 0;
	if(Success)
	{
		VertexID = Holder.Holder.GetSubtype<TWeakPtr<Simulation::Vertex>>().Pin()->GetVertexID();
	}
}

FSimVertexID USimulationFunctionLibrary::GetWayStart(const FGraphWay& Way)
{
	if(!Way.IsValid())
	{
		return FSimVertexID::Invalid;
	}
	return Way.GetStart().Pin()->GetVertexID();
}

FSimVertexID USimulationFunctionLibrary::GetWayEnd(const FGraphWay& Way)
{
	if(!Way.IsValid())
	{
		return FSimVertexID::Invalid;
	}
	return Way.GetEnd().Pin()->GetVertexID();
}

FSimVertexID USimulationFunctionLibrary::GetWayCurrentGraphPoint(const FGraphWay& Way)
{
	if(!Way.IsValid())
	{
		return FSimVertexID::Invalid;
	}
	return Way.GetCurrent().Pin()->GetVertexID();
}

FSimVertexID USimulationFunctionLibrary::GetWayNextGraphPoint(const FGraphWay& Way)
{
	if(!Way.IsValid() || Way.GetCurrent()==Way.GetEnd())
	{
		return FSimVertexID::Invalid;
	}
	return Way.GetNext().Pin()->GetVertexID();
}

FGraphWay USimulationFunctionLibrary::FindWay(UObject* Context, const FSimVertexID& Start, const FSimVertexID& Dest,
	TSet<UNavHeuristic_Base*> Params)
{
	if(!Params.Num())
	{
		return FGraphWay();
	}
	UWorld* World = Context->GetWorld();
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(World);
	auto StartVertex = GlobalGraph->GetVertexByID(Start);
	auto EndVertex = GlobalGraph->GetVertexByID(Dest);
	if(FVector::Dist(StartVertex.Pin()->GetLocation(), EndVertex.Pin()->GetLocation()) >= GetMutableDefault<USimulationSystemSettings>()->FindWayMaxSearchDistance)
	{
		//UE_LOG(LogTemp, Log, TEXT("USimulationFunctionLibrary::Find way: vertex [%s] locates too far from vertex [%s]"), )
		return FGraphWay();
	}
	auto GetScore = [&](const TWeakPtr<Simulation::Vertex>& Current, const TWeakPtr<Simulation::Vertex>& Next)
	{
		double Score = 0;
		for(auto& Param : Params)
		{
			Score = Param->NativeEvaluate(Score, StartVertex, Current, Next, EndVertex);
		}
		return Score;
	};
	TMap<TWeakPtr<Simulation::Vertex>, TPair<TWeakPtr<Simulation::Vertex>, double>> Tree;
	TNativePriorityQueue<TWeakPtr<Simulation::Vertex>> Priority;
	Tree.Add(StartVertex, TPair<TWeakPtr<Simulation::Vertex>, double>{nullptr, 0});
	Priority.Push(StartVertex, 0);
	while(!Priority.IsEmpty())
	{
		auto Current = Priority.Pop();
		if(Current == EndVertex)
		{
			break;
		}
		for(auto& Edge : Current.Pin()->GetEdges())
		{
			auto Next = Edge.Pin()->GetOpposite(Current);
			if(FVector::Dist(StartVertex.Pin()->GetLocation(), Next.Pin()->GetLocation()) >= GetMutableDefault<USimulationSystemSettings>()->FindWayMaxSearchDistance)
			{
				continue;
			}
			double NewScore = Tree[Current].Value + FVector::Dist(Current.Pin()->GetLocation(), Next.Pin()->GetLocation());
			if(!Tree.Contains(Next) || NewScore < Tree[Next].Value)
			{
				double PriorityScore = NewScore + GetScore(Current, Next);
				Priority.Push(Next, PriorityScore);
				Tree.Add(Next, TPair<TWeakPtr<Simulation::Vertex>, double>{Current, NewScore});
			}
		}
	}
	if(!Tree.Contains(EndVertex))
	{
		return FGraphWay();
	}
	FGraphWay NewWay;
	TArray<TWeakPtr<Simulation::Vertex>> Stack;
	Stack.Push(EndVertex);
	auto Node = Tree[EndVertex];
	while(Node.Key.IsValid())
	{
		Stack.Push(Node.Key);
		Node = Tree[Node.Key];
	}
	if (!ensure(Stack.Num() < MAX_PATH_LENGTH))
	{
		return FGraphWay();
	}
	auto& NewWayData = NewWay.GetWay();
	while (Stack.Num())
	{
		NewWayData.Vertices[NewWayData.Num++] = Stack.Pop();
	}
	return NewWay;
}

USimulationSystemSubsystem* USimulationFunctionLibrary::GetSimulationSystemSubsystem(UObject* Context)
{
	return GEngine->GetEngineSubsystem<USimulationSystemSubsystem>();
}

USimulationSystemFunctionsImplementation* USimulationFunctionLibrary::GetFunctions(UObject* Context)
{
	auto Functions = GetSimulationSystemSubsystem(Context)->GetSimulationSystemFunctions();
	if (ensureMsgf(IsValid(Functions), TEXT("Unable to get SimulationSystemFunctions from subsystem!")))
	{
		return Functions;
	}
	return nullptr;
}

void USimulationFunctionLibrary::SaveObjectData(UObject* Object, FBytesSerialization& Data)
{
	FMemoryWriter MemoryWriter(Data.SerializedObject, true);
	FObjectAndNameAsStringProxyArchive SaveArchive(MemoryWriter, false);
	Object->Serialize(SaveArchive);
}

void USimulationFunctionLibrary::LoadObjectData(UObject* Object, const FBytesSerialization& Data)
{
	FMemoryReader MemoryReader(Data.SerializedObject, true);
	FObjectAndNameAsStringProxyArchive SaveArchive(MemoryReader, false);
	Object->Serialize(SaveArchive);
}

UScriptStruct* USimulationFunctionLibrary::GetStructFromName(const FString& StructName)
{
	return FindObject<UScriptStruct>(ANY_PACKAGE, *StructName);
}

bool USimulationFunctionLibrary::GetConstFragmentData(UActionPlannerOwner* Owner, UScriptStruct* Struct, FFragmentProperties& Out)
{
	if (!ensure(Owner))
	{
		return false;
	}
	if (!ensure(Owner->GetProfileType() == ESimualtionSystemProfileType::ECS))
	{
		return false;
	}
	return Owner->GetFragment(Struct, Out);
}

int USimulationFunctionLibrary::GetIntFragmentProperty(const FFragmentProperties& MassFragment, int Index)
{
	if (!ensure(MassFragment.Struct.IsValid()) || !ensure(MassFragment.Props.IsValidIndex(Index)))
	{
		return 0;
	}
	auto Prop = MassFragment.Props[Index];
	auto FieldPtr = Prop->ContainerPtrToValuePtr<void>(MassFragment.Struct.GetMemory());
	if (!ensure(FieldPtr))
	{
		return 0;
	}
	auto IntProp = CastField<FIntProperty>(Prop);
	if (!ensure(IntProp))
	{
		return 0;
	}
	return IntProp->GetPropertyValue(FieldPtr);
}

float USimulationFunctionLibrary::GetFloatFragmentProperty(const FFragmentProperties& MassFragment,
	int Index)
{
	if (!ensure(MassFragment.Struct.IsValid()) || !ensure(MassFragment.Props.IsValidIndex(Index)))
	{
		return 0.0f;
	}
	auto Prop = MassFragment.Props[Index];
	auto FieldPtr = Prop->ContainerPtrToValuePtr<void>(MassFragment.Struct.GetMemory());
	if (!ensure(FieldPtr))
	{
		return 0.0f;
	}
	auto FloatProp = CastField<FFloatProperty>(Prop);
	if (!ensure(FloatProp))
	{
		return 0.0f;
	}
	return FloatProp->GetPropertyValue(FieldPtr);
}

double USimulationFunctionLibrary::GetDoubleFragmentProperty(const FFragmentProperties& MassFragment,
	int Index)
{
	if (!ensure(MassFragment.Struct.IsValid()) || !ensure(MassFragment.Props.IsValidIndex(Index)))
	{
		return 0.0;
	}
	auto Prop = MassFragment.Props[Index];
	auto FieldPtr = Prop->ContainerPtrToValuePtr<void>(MassFragment.Struct.GetMemory());
	if (!ensure(FieldPtr))
	{
		return 0.0;
	}
	auto DoubleProp = CastField<FDoubleProperty>(Prop);
	if (!ensure(DoubleProp))
	{
		return 0.0;
	}
	return DoubleProp->GetPropertyValue(FieldPtr);
}

bool USimulationFunctionLibrary::GetBoolFragmentProperty(const FFragmentProperties& MassFragment, int Index)
{
	if (!ensure(MassFragment.Struct.IsValid()) || !ensure(MassFragment.Props.IsValidIndex(Index)))
	{
		return false;
	}
	auto Prop = MassFragment.Props[Index];
	auto FieldPtr = Prop->ContainerPtrToValuePtr<void>(MassFragment.Struct.GetMemory());
	if (!ensure(FieldPtr))
	{
		return false;
	}
	auto BoolProp = CastField<FBoolProperty>(Prop);
	if (!ensure(BoolProp))
	{
		return false;
	}
	return BoolProp->GetPropertyValue(FieldPtr);
}

FString USimulationFunctionLibrary::GetStringFragmentProperty(const FFragmentProperties& MassFragment,
	int Index)
{
	if (!ensure(MassFragment.Struct.IsValid()) || !ensure(MassFragment.Props.IsValidIndex(Index)))
	{
		return {};
	}
	auto Prop = MassFragment.Props[Index];
	auto FieldPtr = Prop->ContainerPtrToValuePtr<void>(MassFragment.Struct.GetMemory());
	if (!ensure(FieldPtr))
	{
		return {};
	}
	auto StrProp = CastField<FStrProperty>(Prop);
	if (!ensure(StrProp))
	{
		return {};
	}
	return StrProp->GetPropertyValue(FieldPtr);
}

FName USimulationFunctionLibrary::GetNameFragmentProperty(const FFragmentProperties& MassFragment,
	int Index)
{
	if (!ensure(MassFragment.Struct.IsValid()) || !ensure(MassFragment.Props.IsValidIndex(Index)))
	{
		return NAME_None;
	}
	auto Prop = MassFragment.Props[Index];
	auto FieldPtr = Prop->ContainerPtrToValuePtr<void>(MassFragment.Struct.GetMemory());
	if (!ensure(FieldPtr))
	{
		return NAME_None;
	}
	auto NameProp = CastField<FNameProperty>(Prop);
	if (!ensure(NameProp))
	{
		return NAME_None;
	}
	return NameProp->GetPropertyValue(FieldPtr);
}

FText USimulationFunctionLibrary::GetTextFragmentProperty(const FFragmentProperties& MassFragment,
	int Index)
{
	if (!ensure(MassFragment.Struct.IsValid()) || !ensure(MassFragment.Props.IsValidIndex(Index)))
	{
		return {};
	}
	auto Prop = MassFragment.Props[Index];
	auto FieldPtr = Prop->ContainerPtrToValuePtr<void>(MassFragment.Struct.GetMemory());
	if (!ensure(FieldPtr))
	{
		return {};
	}
	auto TextProp = CastField<FTextProperty>(Prop);
	if (!ensure(TextProp))
	{
		return {};
	}
	return TextProp->GetPropertyValue(FieldPtr);
}

UObject* USimulationFunctionLibrary::GetObjectFragmentProperty(const FFragmentProperties& MassFragment,
	int Index)
{
	if (!ensure(MassFragment.Struct.IsValid()) || !ensure(MassFragment.Props.IsValidIndex(Index)))
	{
		return nullptr;
	}
	auto Prop = MassFragment.Props[Index];
	auto FieldPtr = Prop->ContainerPtrToValuePtr<void>(MassFragment.Struct.GetMemory());
	if (!ensure(FieldPtr))
	{
		return nullptr;
	}
	auto ObjProp = CastField<FObjectProperty>(Prop);
	if (!ensure(ObjProp))
	{
		return nullptr;
	}
	return ObjProp->GetPropertyValue(FieldPtr);
}

UClass* USimulationFunctionLibrary::GetClassFragmentProperty(const FFragmentProperties& MassFragment,
	int Index)
{
	if (!ensure(MassFragment.Struct.IsValid()) || !ensure(MassFragment.Props.IsValidIndex(Index)))
	{
		return nullptr;
	}
	auto Prop = MassFragment.Props[Index];
	auto FieldPtr = Prop->ContainerPtrToValuePtr<void>(MassFragment.Struct.GetMemory());
	if (!ensure(FieldPtr))
	{
		return nullptr;
	}
	auto ClassProp = CastField<FClassProperty>(Prop);
	if (!ensure(ClassProp))
	{
		return nullptr;
	}
	return Cast<UClass>(ClassProp->GetPropertyValue_InContainer(FieldPtr));
}

void USimulationFunctionLibrary::GetStructFragmentProperty(const FFragmentProperties& MassFragment,
	int Index, FInstancedStruct& Out)
{
	if (!ensure(MassFragment.Struct.IsValid()) || !ensure(MassFragment.Props.IsValidIndex(Index)))
	{
		return;
	}
	auto Prop = MassFragment.Props[Index];
	auto FieldPtr = Prop->ContainerPtrToValuePtr<void>(MassFragment.Struct.GetMemory());
	if (!ensure(FieldPtr))
	{
		return;
	}
	auto StructProp = CastField<FStructProperty>(Prop);
	if (!ensure(StructProp))
	{
		return;
	}
	Out.InitializeAs(MassFragment.Struct.GetScriptStruct());
	StructProp->Struct->CopyScriptStruct(Out.GetMutableMemory(), FieldPtr);
}

