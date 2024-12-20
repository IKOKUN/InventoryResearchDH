// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor/MissionActor.h"
#include "Controller/IRPlayerController.h"
#include "Character/IRCharacter.h"
#include "InventoryComponent/EquipmentInventoryComponent.h"
#include "Data/InventoryItem.h"

bool AMissionActor::OnActorUsed(APlayerController* PlayerController)
{
	SetOwner(PlayerController->GetPawn());
	InteractorPlayerController = Cast<AIRPlayerController>(PlayerController);
	InteractorPlayerCharacter = Cast<AIRCharacter>(InteractorPlayerController->GetPawn());

	if (!bWasUsed)
	{
		TArray<FInventoryItem> ALlPlayerItems = InteractorPlayerController->GetPlayerInventoryComponent()->GetInventoryItems();
		bool bFoundQuestItem = false;
		for (FInventoryItem PlayerItem : ALlPlayerItems)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Item Name : %s"), *PlayerItem.ID.ToString());
			if (PlayerItem.ItemType == EItemType::QuestItem)
			{
				bFoundQuestItem = true;
				break;				
			}
		}
		if (bFoundQuestItem)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Quest Item Found, Please Use Key"));
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Quest Item Not Found, Please Find Key"));
			}
			bWasUsed = false;
		}
	}
	return true;
}

bool AMissionActor::OnActorInspect(APlayerController* PlayerController)
{
	return Super::OnActorInspect(PlayerController);
}

bool AMissionActor::BeginOutlineFocus()
{
	return Super::BeginOutlineFocus();
}

bool AMissionActor::EndOutlineFocus()
{
	return Super::EndOutlineFocus();
}

bool AMissionActor::GetIsActorUsable()
{
	return Super::GetIsActorUsable();
}

FText AMissionActor::GetUseActionText()
{
	return Super::GetUseActionText();
}
