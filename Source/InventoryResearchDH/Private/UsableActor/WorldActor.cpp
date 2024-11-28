// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor/WorldActor.h"
#include "Controller/IRPlayerController.h"
#include "InventoryComponent/InventoryManagerComponent.h"
#include "InventoryComponent/InventoryComponent.h"
#include "InventoryComponent/EquipmentInventoryComponent.h"
#include "Data/InventoryItem.h"

AWorldActor::AWorldActor()
{
	ActionText = FText::FromString("Pickup");
}

bool AWorldActor::LoadItemFromList()
{
    if (GetDataTableRowByName(DataTable, ID, InventoryItem))
    {
        // Log untuk memastikan item diambil dengan benar
        UE_LOG(LogTemp, Log, TEXT("Successfully loaded item: %s"), *InventoryItem.Name.ToString());

        // Set nama item
        Name = InventoryItem.Name;

        // Log semua atribut dari InventoryItem
        /*UE_LOG(LogTemp, Log, TEXT("ID: %s"), *InventoryItem.ID.ToString());
        UE_LOG(LogTemp, Log, TEXT("Icon: %s"), InventoryItem.Icon ? *InventoryItem.Icon->GetName() : TEXT("None"));
        UE_LOG(LogTemp, Log, TEXT("Description: %s"), *InventoryItem.Description);
        UE_LOG(LogTemp, Log, TEXT("Quality: %d"), (int32)InventoryItem.Quality);
        UE_LOG(LogTemp, Log, TEXT("ItemType: %d"), (int32)InventoryItem.ItemType);
        UE_LOG(LogTemp, Log, TEXT("Amount: %d"), InventoryItem.Amount);
        UE_LOG(LogTemp, Log, TEXT("bIsStackable: %s"), InventoryItem.bIsStackable ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Log, TEXT("MaxStackSize: %d"), InventoryItem.MaxStackSize);
        UE_LOG(LogTemp, Log, TEXT("bIsDroppable: %s"), InventoryItem.bIsDroppable ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Log, TEXT("Health: %f"), InventoryItem.Health);
        UE_LOG(LogTemp, Log, TEXT("Duration: %f"), InventoryItem.Duration);*/
        // Tambahkan log untuk atribut lainnya jika diperlukan

        // Pastikan atribut lainnya diatur dengan benar
        InventoryItem.bIsStackable = true; // Atur apakah item dapat di-stack
        InventoryItem.MaxStackSize = 5; // Misalnya, atur batas maksimum stack

        if (InventoryItem.WorldMesh)
        {
            return true; // Item berhasil diambil dan memiliki mesh dunia
        }
        else
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    5.0f,
                    FColor::Green,
                    FString::Printf(TEXT("Item '%s' has no World Mesh"), *InventoryItem.Name.ToString())
                );
            }
        }
    }
    else
    {
        // Log jika item tidak ditemukan di data table
        UE_LOG(LogTemp, Error, TEXT("Failed to load item with ID: %s"), *ID.ToString());
    }
    return false; // Item tidak berhasil diambil
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

		bool bSucces = IRPlayerController->GetInventoryManagerComponent()->TryToAddItemToInventory(InventoryComp, InventoryItem);
		if (bSucces)
		{
			Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed Add Item To Inventory"));
		}
		Super::OnActorUsed(PlayerController);
		return true;
	}
	
	return false;
}
