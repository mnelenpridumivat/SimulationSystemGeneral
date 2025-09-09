// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Profiles\AISimProfileSquad.h"

#include "GlobalGraph.h"
#include "GraphSerialized.h"
#include "SimulationFunctionLibrary.h"
#include "SquadTaskBase.h"
#include "EntitySystem/MovieSceneEntityManager.h"
#include "Profiles/AISimProfilePawn.h"

void UAISimProfileSquad::OnCreated_Implementation()
{
	Super::OnCreated_Implementation();
	RefreshSquadCharacteristics();
}

void UAISimProfileSquad::OnLoaded_Implementation()
{
	Super::OnLoaded_Implementation();
	RefreshSquadCharacteristics();
}

USimProfileBase* UAISimProfileSquad::DeepCopyProfile()
{
	auto Copy = Cast<UAISimProfileSquad>(Super::DeepCopyProfile());
	for(int i = 0; i < Copy->Members.Num(); ++i)
	{
		Copy->Members[i] = Cast<UAISimProfilePawn>(Copy->Members[i]->DeepCopyProfile());
	}
	return Copy;
}

void UAISimProfileSquad::AddItem_Implementation(USimProfileBase* Profile)
{
	ensureMsgf(ISimProfileContainer::Execute_CanStoreItem(this, Profile), TEXT("Unable to put profile [%s] in profile [%s]"), *Profile->GetName(), *GetName());
	Members.Add(Cast<UAISimProfilePawn>(Profile));
}

TArray<USimProfileBase*> UAISimProfileSquad::GetAllItems_Implementation()
{
	TArray<USimProfileBase*> OutArray;
	OutArray.Reserve(Members.Num());
	for(auto& elem : Members)
	{
		OutArray.Add(elem);
	}
	return OutArray;
}

bool UAISimProfileSquad::HasItem_Implementation(USimProfileBase* Profile)
{
	ensureMsgf(ISimProfileContainer::Execute_CanStoreItem(this, Profile), TEXT("Profile [%s] cannot contain profile [%s] because cannot contain such class"), *GetName(), *Profile->GetName());
	return Members.Contains(Cast<UAISimProfilePawn>(Profile));
}

void UAISimProfileSquad::RemoveItem_Implementation(USimProfileBase* Profile)
{
	ensureMsgf(ISimProfileContainer::Execute_CanStoreItem(this, Profile), TEXT("Profile [%s] cannot contain profile [%s] because cannot contain such class"), *GetName(), *Profile->GetName());
	Members.Remove(Cast<UAISimProfilePawn>(Profile));
}

bool UAISimProfileSquad::CanStoreItem_Implementation(USimProfileBase* Profile)
{
	return Profile->IsA(UAISimProfilePawn::StaticClass());
}

void UAISimProfileSquad::OnRegistered_Implementation()
{
	Super::OnRegistered_Implementation();
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	for(auto& Member : Members)
	{
		GlobalGraph->RegisterChildProfile(Member, this);
	}
}

void UAISimProfileSquad::SetOnlineLocation(FVector Vector)
{
	Super::SetOnlineLocation(Vector);
	for(auto& Member : Members)
	{
		Member->SetOnlineLocation(Vector);
	}
}

void UAISimProfileSquad::Save_Implementation(FSimVertexID VertexID, FSerializedProfileView Data)
{
	Super::Save_Implementation(VertexID, Data);
	Data.GetElem().NextSet();
	for(auto Item : Members)
	{
		Item->Save(VertexID, Data.GetElem().AddChild());
	}
	Data.GetElem().NextSet();
	if (CurrentTask)
	{
		CurrentTask->Save(Data.GetElem().AddChild());
	}
}

void UAISimProfileSquad::Load_Implementation(FSerializedProfile& Data)
{
	Super::Load_Implementation(Data);
	{
		FProfilesSerializedView Children;
		Data.ExtractFirstChildren(Children);
		for(auto elem : Children.Objects)
		{
			auto Member = NewObject<UAISimProfilePawn>(GetWorld(), elem->ObjectClass);
			Member->Load(*elem);
			Execute_AddItem(this, Member);
		}
	}
	{
		FProfilesSerializedView Children;
		Data.ExtractFirstChildren(Children);
		if(ensure(Children.Objects.Num() <= 1) && Children.Objects.Num() == 1)
		{			
			if(const auto TaskData = Children.Objects[0];
				ensure(TaskData->ObjectClass->IsChildOf(USquadTaskBase::StaticClass())))
			{
				auto NewTask = NewObject<USquadTaskBase>(TaskData->ObjectClass);
				USimulationFunctionLibrary::LoadObjectData(NewTask, TaskData->ObjectData);
				SetCurrentTask(NewTask);
			}
		}		
	}
}

void UAISimProfileSquad::RefreshSquadCharacteristics()
{
	for(auto& Value : CharacteristicsList)
	{
		Value.Value = 0;
	}
	for(auto& Value : PointsList)
	{
		Value.Value = 0;
	}
	if(!Members.Num())
	{
		return;
	}
	for(auto& Member : Members)
	{
		auto& MemberChars = Member->GetCharacteristics();
		for(auto& Value : CharacteristicsList)
		{
			Value.Value += MemberChars[Value.Key];
		}
		auto& MemberPoints = Member->GetPoints();
		for(auto& Value : PointsList)
		{
			Value.Value += MemberPoints[Value.Key];
		}
	}
	for(auto& Value : CharacteristicsList)
	{
		Value.Value = static_cast<int>(FMath::Floor(static_cast<float>(Value.Value) / Members.Num()));
	}
	for(auto& Value : PointsList)
	{
		Value.Value = FMath::Floor(Value.Value / Members.Num());
	}
}

float UAISimProfileSquad::GetOfflineSpeed()
{
	return 100.0f;
}

void UAISimProfileSquad::Tick_Implementation(float DeltaTime)
{
	Super::Tick_Implementation(DeltaTime);
	
	if(!CurrentTask)
	{
		CurrentTask = USimulationFunctionLibrary::GetGlobalGraph(GetWorld())->CreateNewTask(this);
		if(!CurrentTask)
		{
			return;
		}
		CurrentTask->BeginTask(this);
	}
	if(CurrentTask->CheckTaskFinished(this))
	{
		CurrentTask->FinishTask(this, true);
		CurrentTask = nullptr;
	} else
	{
		CurrentTask->ExecuteTask(this, DeltaTime);
	}
}
