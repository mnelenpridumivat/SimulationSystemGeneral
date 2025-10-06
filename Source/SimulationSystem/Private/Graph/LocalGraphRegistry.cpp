// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalGraphRegistry.h"

#include "GraphAsset.h"
#include "SimProfileBase.h"

void ULocalGraphRegistry::SetLocalGraph(AGraphAsset* GraphAsset)
{
	if (!ensureMsgf(
		IsValid(GraphAsset),
		TEXT("Attempt to set invalid ParentGraphAsset to registry [%s]"),
		*GetName()))
	{
		return;
	}
	ensureMsgf(
		!IsValid(ParentGraphAsset),
		TEXT("Attempt to set ParentGraphAsset [%s] to registry [%s] while other ParentGraphAsset [%s] is set!"),
		*(GraphAsset->GetName()),
		*GetName(),
		*(ParentGraphAsset->GetName()));
	ParentGraphAsset = GraphAsset;
}

void ULocalGraphRegistry::NativeRegisterProfile(USimProfileBase* Profile)
{
	if (!ensureMsgf(
			IsValid(ParentGraphAsset),
			TEXT("Unable to process RegisterProfile [%s] in LocalGraphRegistry [%s] with invalid ParentGraphAsset!"),
			*(Profile->GetName()),
			*GetName())
		|| !ensureMsgf(
			IsValid(Profile),
			TEXT("Attempt to call RegisterProfile with invalid profile in LocalGraphRegistry [%s] inside [%s]"),
			*GetName(),
			*(ParentGraphAsset->GetName())))
	{
		return;
	}
	if (ensureMsgf(
		!ProfilesInside.Contains(Profile),
		TEXT("Attempt to register profile [%s] second time in local graph [%s]"),
		*(Profile->GetName()),
		*(ParentGraphAsset->GetName())))
	{
		ProfilesInside.Add(Profile);
		RegisterProfile(Profile);
		OnProfileEntered.Broadcast(Profile);
	}
}

void ULocalGraphRegistry::NativeUnregisterProfile(USimProfileBase* Profile)
{
	if (!ensureMsgf(
			IsValid(ParentGraphAsset),
			TEXT("Unable to process UnregisterProfile [%s] in LocalGraphRegistry [%s] with invalid ParentGraphAsset!"),
			*(Profile->GetName()),
			*GetName())
		|| !ensureMsgf(
			IsValid(Profile),
			TEXT("Attempt to call UnregisterProfile with invalid profile in LocalGraphRegistry [%s] inside [%s]"),
			*GetName(),
			*(ParentGraphAsset->GetName())))
	{
		return;
	}
	if (ensureMsgf(
		ProfilesInside.Contains(Profile),
		TEXT("Attempt to unregister profile [%s] in [%s] which is not in registry!"),
		*(Profile->GetName()),
		*(ParentGraphAsset->GetName())))
	{
		ProfilesInside.Remove(Profile);
		UnregisterProfile(Profile);
		OnProfileExited.Broadcast(Profile);
	}
}

void ULocalGraphRegistry::GetAllProfiles(TArray<USimProfileBase*>& Profiles)
{
	Profiles.Empty();
	for (auto Inside : ProfilesInside)
	{
		Profiles.Add(Inside);
	}
}
