// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationState.h"

#include "AISimProfileSquad.h"
#include "SimulationFunctionLibrary.h"
#include "SquadTaskBase.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void USimulationState::SaveProfile(USimProfileBase* Profile, /*uint32 ParentIndex, uint32& Counter, */FSimVertexID VertexID)
{
	FSerializedProfileView Serialized = Profiles.AddLast();
	Profile->Save(VertexID, Serialized);
}

USimProfileBase* USimulationState::LoadProfile(int Index)
{
	auto& ProfileData = Profiles.Objects[Index];
	USimProfileBase* Profile = NewObject<USimProfileBase>(GetWorld(), ProfileData.ObjectClass);
	USimulationFunctionLibrary::LoadObjectData(Profile, ProfileData.ObjectData);
	Profile->Load(ProfileData);
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
