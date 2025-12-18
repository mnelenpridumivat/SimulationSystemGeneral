// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorToolRunHelper.h"

void UEditorToolRunHelper::OnDelayComplete()
{
	if (ensure(OnDelay))
	{
		OnDelay();
	}
	RemoveFromRoot();
}
