// Fill out your copyright notice in the Description page of Project Settings.


#include "ProfileIDController.h"

#include "ReplicatedSimInfo.h"
#include "SimProfileBase.h"


const FSimProfileID UProfileIDController::InvalidID = {nullptr, static_cast<uint32>(-1)};

FSimProfileID UProfileIDController::RegisterProfile(USimProfileBase* SimProfileBase)
{
	FSimProfileID ID = SimProfileBase->GetProfileID();
	if(ID!=InvalidID)
	{
		auto ClassMap = ProfilesByID.Find(SimProfileBase->GetProfileID().Class);
		ensureMsgf(!ClassMap || !ClassMap->Profiles.Contains(ID.ProfileID), TEXT("Duplicated ProfileID found!"));
		if(!ClassMap)
		{
			ClassMap = &ProfilesByID.Add(ID.Class, FSimProfilesByID());
		}
		ClassMap->Profiles.Add(SimProfileBase->GetProfileID().ProfileID, SimProfileBase);
		return ID;
	}
	TSubclassOf<USimProfileBase> ProfileClass = SimProfileBase->GetClass();
	if(!MaxUsedIDByClass.Contains(ProfileClass))
	{
		MaxUsedIDByClass.Add(ProfileClass, 0);
	}
	if(!FreeIDs.Contains(ProfileClass))
	{
		FreeIDs.Add(ProfileClass);
	}
	FSimProfileID NewID = {ProfileClass, static_cast<uint32>(-1)};
	auto& FreeIDsRef = FreeIDs[ProfileClass].FreeID;
	if(FreeIDsRef.Num())
	{
		NewID.ProfileID = FreeIDsRef.Pop();
	} else
	{
		NewID.ProfileID = (*MaxUsedIDByClass.Find(ProfileClass))++;
	}
	auto ClassMap = ProfilesByID.Find(NewID.Class);
	if(!ClassMap)
	{
		ClassMap = &ProfilesByID.Add(NewID.Class, FSimProfilesByID());
	}
	ClassMap->Profiles.Add(NewID.ProfileID, SimProfileBase);
	return NewID;
}

USimProfileBase* UProfileIDController::GetProfile(const FSimProfileID& ID)
{
#ifndef NoSimEnsureMessage
	ensureMsgf(ID.IsValid(), TEXT("Cannot get profile from invalid ID"));
#endif
	auto IDinMap = ProfilesByID.Find(ID.Class);
	if(!IDinMap)
	{
		return nullptr;
	}
	auto ProfilePtr = IDinMap->Profiles.Find(ID.ProfileID);
	return ProfilePtr ? *ProfilePtr : nullptr;
}

TArray<USimProfileBase*> UProfileIDController::GetProfiles(TSubclassOf<USimProfileBase> Class)
{
	TArray<USimProfileBase*> FoundProfiles;
	for(auto& elem : ProfilesByID)
	{
		if(elem.Key->IsChildOf(Class))
		{
			TArray<USimProfileBase*> ValueArr;
			elem.Value.Profiles.GenerateValueArray(ValueArr);
			FoundProfiles.Append(ValueArr);
		}
	}
	return FoundProfiles;
}

void UProfileIDController::AddReplicatedSimInfo(AReplicatedSimInfo* ReplicatedSimInfo)
{
	ReplicatedProfilesData.Add(ReplicatedSimInfo->GetProfileID(), ReplicatedSimInfo);
}

void UProfileIDController::RemoveReplicatedSimInfo(AReplicatedSimInfo* ReplicatedSimInfo)
{
	ReplicatedProfilesData.Remove(ReplicatedSimInfo->GetProfileID());
}

AReplicatedSimInfo* UProfileIDController::GetReplicatedSimInfo(FSimProfileID ProfileID)
{
	auto Info = ReplicatedProfilesData.Find(ProfileID);
#if WITH_EDITOR
	if(!Info)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find replicated profile info with ID [%s, %u]!"), *ProfileID.Class->GetName(), ProfileID.ProfileID);
		return nullptr;
	}
#else
	ensureMsgf(Info, TEXT("Unable to find replicated profile info with ID [%s, %u]!"), *ProfileID.Class->GetName(), ProfileID.ProfileID);
#endif
	
	return *Info;
}
