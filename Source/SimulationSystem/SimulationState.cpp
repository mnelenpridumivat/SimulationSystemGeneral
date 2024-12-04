// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationState.h"

#include "AISimProfileSquad.h"
#include "SimulationFunctionLibrary.h"
#include "SquadTaskBase.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void USimulationState::SaveProfile(USimProfileBase* Profile, /*uint32 ParentIndex, uint32& Counter, */FSimVertexID VertexID)
{
	//auto CurrentIndex = Counter++;
	FSerializedProfile Serialized;
	Serialized.ObjectClass = Profile->GetClass();
	Serialized.VertexLocation = VertexID;
	USimulationFunctionLibrary::SaveObjectData(Profile, Serialized.ObjectData.SerializedObject);
	if(Profile->GetClass()->ImplementsInterface(USimProfileContainer::StaticClass()))
	{
		auto ChildProfiles = ISimProfileContainer::Execute_GetAllItems(Profile);
		Serialized.ChildrenNum = ChildProfiles.Num();
		Profiles.Objects.Add(Serialized);
		for(auto& Child : ChildProfiles)
		{
			SaveProfile(Child);
		}
	} else
	{
		Profiles.Objects.Add(Serialized);
	}
	if(Profile->IsA(UAISimProfileSquad::StaticClass()))
	{
		auto Task = Cast<UAISimProfileSquad>(Profile)->GetCurrentTask();
		Serialized.ObjectClass = Task->GetClass();
		USimulationFunctionLibrary::SaveObjectData(Task, Serialized.ObjectData.SerializedObject);
		Profiles.Objects.Add(Serialized);
	}
	if(Profile->HasOnlineActor() && Profile->GetSimLevel()==ESimulationLevels_Online)
	{
		AActor* OnlineActor = Profile->GetOnlineActor();
		Serialized.ObjectClass = OnlineActor->GetClass();
		USimulationFunctionLibrary::SaveObjectData(OnlineActor, Serialized.ObjectData.SerializedObject);
		Profiles.Objects.Add(Serialized);
	}
}

USimProfileBase* USimulationState::LoadProfile(int& Index)
{
	USimProfileBase* Profile = NewObject<USimProfileBase>(GetWorld(), Profiles.Objects[Index].ObjectClass);
	USimulationFunctionLibrary::LoadObjectData(Profile, Profiles.Objects[Index].ObjectData.SerializedObject);
	if(Profile->GetClass()->ImplementsInterface(USimProfileContainer::StaticClass()))
	{
		auto ChildrenNum = Profiles.Objects[Index].ChildrenNum;
		for(uint32 i = 0; i < ChildrenNum; ++i)
		{
			ISimProfileContainer::Execute_AddItem(Profile, LoadProfile(++Index));
		}
	}
	if(Profile->IsA(UAISimProfileSquad::StaticClass()))
	{
		auto Task = NewObject<USquadTaskBase>(GetWorld(), Profiles.Objects[++Index].ObjectClass);
		USimulationFunctionLibrary::LoadObjectData(Task, Profiles.Objects[Index].ObjectData.SerializedObject);
		Cast<UAISimProfileSquad>(Profile)->SetCurrentTask(Task);
	}
	if(Profile->HasOnlineActor() && Profile->GetSimLevel()==ESimulationLevels_Online)
	{
		auto OnlineActor = GetWorld()->SpawnActor(Profiles.Objects[++Index].ObjectClass);
		USimulationFunctionLibrary::LoadObjectData(OnlineActor, Profiles.Objects[Index].ObjectData.SerializedObject);
		Profile->SetOnlineActor(OnlineActor);
	}
	Profile->OnLoaded();
	return Profile;
}

TMap<FSimVertexID, USimProfileBase*> USimulationState::GetProfiles()
{
	TMap<FSimVertexID, USimProfileBase*> Loaded;
	for(int i = 0; i < Profiles.Objects.Num(); ++i)
	{
		auto LoadedProfile = LoadProfile(i);
		//LoadedProfile->
		Loaded.Add(Profiles.Objects[i].VertexLocation, LoadedProfile);
	}
	return Loaded;
}

void USimulationState::SetProfiles(const TMap<FSimVertexID, USimProfileBase*>& NewProfiles)
{
	uint32 SaveIndex = 0;
	for(auto& elem : NewProfiles)
	{
		SaveProfile(elem.Value, /*-1, SaveIndex, */elem.Key);
	}
}
