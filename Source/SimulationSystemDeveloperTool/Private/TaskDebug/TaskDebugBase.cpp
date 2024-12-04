// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskDebug/TaskDebugBase.h"

#include <imgui.h>

#include "SquadTaskBase.h"

void UTaskDebugBase::VisualizeTaskInfo(float DeltaTime, UAISimProfileSquad* Squad, USquadTaskBase* Task)
{
}

void UTaskDebugBase::ImGuiTaskInfo(float DeltaTime, UAISimProfileSquad* Squad, USquadTaskBase* Task)
{
	FString TaskName = Task->GetName();
	FString TaskProgress;
	if(Task->IsAborted())
	{
		TaskProgress = "Status: Aborted";
	} else if (Task->CheckTaskFinished(Squad))
	{
		TaskProgress = "Status: Finished";
	} else
	{
		TaskProgress = "Status: In progress";
	}
	ImGui::Text(TCHAR_TO_ANSI(*TaskName));
	ImGui::Text(TCHAR_TO_ANSI(*TaskProgress));
}
