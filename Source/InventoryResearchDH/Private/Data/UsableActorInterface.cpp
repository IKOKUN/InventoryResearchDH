// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/UsableActorInterface.h"

// Add default functionality here for any IUsableActorInterface functions that are not pure virtual.

bool IUsableActorInterface::OnActorUsed(APlayerController* PlayerController)
{
	return false;
}

bool IUsableActorInterface::BeginOutlineFocus()
{
	return false;
}

bool IUsableActorInterface::EndOutlineFocus()
{
	return false;
}

bool IUsableActorInterface::GetIsActorUsable()
{
	return false;
}

FText IUsableActorInterface::GetUseActionText()
{
	return FText();
}

