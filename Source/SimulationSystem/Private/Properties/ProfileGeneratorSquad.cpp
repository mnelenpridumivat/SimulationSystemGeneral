// Fill out your copyright notice in the Description page of Project Settings.


#include "Properties/ProfileGeneratorSquad.h"

#include "AISimProfilePawn.h"
#include "AISimProfileSquad.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemSettings.h"
#include "SimulationSystemSubsystem.h"

void FGeneratorHandleSquad::SetupGenerator(UProfileGenerator* Generator) const
{
	auto CastedGenerator = Cast<UProfileGeneratorSquad>(Generator);
	if (!ensureMsgf(IsValid(CastedGenerator),
		TEXT("GeneratorHandle (Squad): expected generator of class UProfileGeneratorSquad, but received [%s"),
		*Generator->GetClass()->GetName()))
	{
		return;
	}
	CastedGenerator->SetSquadName(SquadName.Name);
}

USimProfileBase* UProfileGeneratorSquad::GenerateProfile()
{
	auto Settings = GetDefault<USimulationSystemSettings>();
	if (!ensureMsgf(!Settings->Squads.IsNull(), TEXT("Squads DataTable not found!")))
	{
		return nullptr;
	}
	auto SquadData = Settings->Squads.LoadSynchronous()->FindRow<FSquadData>(SquadName, nullptr);
	if (!ensureMsgf(SquadData, TEXT("Unable to find squad data [%s]"), *SquadName.ToString()))
	{
		return nullptr;
	}
	auto NewSquad = NewObject<UAISimProfileSquad>(GetWorld(), UAISimProfileSquad::StaticClass());

	if (SquadData->Random)
	{
		auto Num = FMath::RandRange(SquadData->Min, SquadData->Max);
		for (int i = 0; i < Num; i++)
		{
			const auto& NPCs = SquadData->NPCs;
			auto NPC = NPCs[FMath::RandRange(0, NPCs.Num()-1)].Name;
			auto Pawn = USimulationFunctionLibrary::GetFunctions(GetWorld())->SpawnPawn(NPC);
			if (ensureMsgf(IsValid(Pawn), TEXT("Unable to spawn pawn [%s]"), *NPC.ToString()))
			{
				ISimProfileContainer::Execute_AddItem(NewSquad, Pawn);
			}
		}
	} else
	{
		for (auto NPC : SquadData->NPCs)
		{
			auto Pawn = USimulationFunctionLibrary::GetFunctions(GetWorld())->SpawnPawn(NPC.Name);
			if (ensureMsgf(IsValid(Pawn), TEXT("Unable to spawn pawn [%s]"), *NPC.Name.ToString()))
			{
				ISimProfileContainer::Execute_AddItem(NewSquad, Pawn);
			}
		}
	}

	return NewSquad;
}
