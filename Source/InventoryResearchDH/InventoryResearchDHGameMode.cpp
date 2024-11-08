// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryResearchDHGameMode.h"
#include "InventoryResearchDHCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInventoryResearchDHGameMode::AInventoryResearchDHGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
