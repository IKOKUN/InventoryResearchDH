// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor/WorldActor.h"
#include "Controller/IRPlayerController.h"
#include "InventoryComponent/InventoryManagerComponent.h"
#include "InventoryComponent/InventoryComponent.h"
#include "InventoryComponent/EquipmentInventoryComponent.h"
#include "Data/InventoryItem.h"

AWorldActor::AWorldActor()
{

}

bool AWorldActor::LoadItemFromList()
{
	FInventoryItem* Item = ItemList->FindRow<FInventoryItem>(ID, "");
	if (Item)
	{
		//Name = FName(*Item->Name);
		Name = Item->Name;
		if (Item->WorldMesh)
		{
			return true;
		}
	}
	return false;
}

void AWorldActor::UpdateItemAmount()
{
	int32 LocalAmount = Amount;
	if (InventoryItem.Amount != LocalAmount)
	{
		if (LocalAmount > InventoryItem.Amount)
		{
			LocalAmount = InventoryItem.MaxStackSize;
		}
	}
	InventoryItem.Amount = LocalAmount;
}

void AWorldActor::BeginPlay()
{
	Super::BeginPlay();

	LoadItemFromList();
	UpdateItemAmount();
}

bool AWorldActor::OnActorUsed(APlayerController* PlayerController)
{
	AIRPlayerController* IRPlayerController = Cast<AIRPlayerController>(PlayerController);
	if (IRPlayerController)
	{
		UInventoryComponent* InventoryComp = CastChecked<UInventoryComponent>(IRPlayerController->GetPlayerInventoryComponent(), ECastCheckedType::NullChecked);

		bool bSucces = IRPlayerController->GetInventoryManagerComponent()->TryAddItemToInventory(InventoryComp, InventoryItem);
		if (bSucces)
		{
			Destroy();
            
		}
		Super::OnActorUsed(PlayerController);
		return true;
	}
	
	return false;
}
