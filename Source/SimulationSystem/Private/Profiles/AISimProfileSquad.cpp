// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Profiles\AISimProfileSquad.h"

#include "GlobalGraph.h"
#include "SimulationFunctionLibrary.h"
#include "SquadTaskBase.h"
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

void UAISimProfileSquad::RefreshSquadCharacteristics()
{
	for(auto& Value : Characteristics)
	{
		Value.Value = 0;
	}
	for(auto& Value : Points)
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
		for(auto& Value : Characteristics)
		{
			Value.Value += MemberChars[Value.Key];
		}
		auto& MemberPoints = Member->GetPoints();
		for(auto& Value : Points)
		{
			Value.Value += MemberPoints[Value.Key];
		}
	}
	for(auto& Value : Characteristics)
	{
		Value.Value = static_cast<int>(FMath::Floor(static_cast<float>(Value.Value) / Members.Num()));
	}
	for(auto& Value : Points)
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
