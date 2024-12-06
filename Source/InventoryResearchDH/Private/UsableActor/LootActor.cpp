// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor/LootActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"

ALootActor::ALootActor()
{
	SlotsPerRow = 1;
	bCanStoreItem = false;
	ActionText = FText::FromString("Loot");
}

TArray<FName> ALootActor::GetDataTableRowNames(UDataTable* SrcDataTable)
{
	TArray<FName> RowNames;
	if (!SrcDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
		return RowNames;
	}

	// Mendapatkan semua nama baris dari Data Table
	RowNames = SrcDataTable->GetRowNames();

	return RowNames;
}

void ALootActor::BeginPlay()
{
	Super::BeginPlay();
}

bool ALootActor::InitInventory()
{
	Super::InitInventory();

	TArray<FInventoryItem> RandomInventoryItemArray;
	RandomInventoryItemArray = GetRandomLootItems();

	//UE_LOG(LogTemp, Warning, TEXT("Anjay Mabar"));
	/*for (FInventoryItem InvItem : RandomInventoryItemArray)
	{
		UE_LOG(LogTemp, Log, TEXT("LoadInventoryItems In Container Actor: Item ID: %s, Amount: %d"), *InvItem.ID.ToString(), InvItem.Amount);
	}*/

	return LoadInventoryItems(RandomInventoryItemArray.Num(), RandomInventoryItemArray);
}

TArray<FLootList> ALootActor::GetLootList()
{
	TArray<FName> RowNames;
	TArray<FLootList> LootListArray;
	FLootList LootListTemp;
	RowNames = GetDataTableRowNames(DataTableLootCommon);
	for (FName RowName : RowNames)
	{
		if (GetDataTableRowByName(DataTableLootCommon, RowName, LootListTemp))
		{
			LootListArray.Add(LootListTemp);
		}
	}
	return LootListArray;
}

TArray<FInventoryItem> ALootActor::GetRandomLootItems()
{
	FLootList LootListTemp;
	FInventoryItem InventoryItemTemp;
	TArray<FLootList> LootListArray;
	TArray<FInventoryItem> InventoryItemArray;
	int32 LocalLootAmount = UKismetMathLibrary::RandomIntegerInRange(MinLootItems, MaxLootItems);
	int32 LocalLootCount = 0;
	int32 LocalItemIndex = 0;
	TArray<int32> LocalItemArray;
	int32 LocalItemAmount = 1;
	
	LootListArray = GetLootList();

	/*for (FLootList CurrentLootList : LootListArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loot List ID: %s"), *CurrentLootList.ID.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Loot List Min Amount: %d"), CurrentLootList.MinAmount);
		UE_LOG(LogTemp, Warning, TEXT("Loot List Max Amount: %d"), CurrentLootList.MaxAmount);
	}*/

	while (LocalLootCount < LocalLootAmount)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Local Loot Count < Local Loot Amount"));
		LocalItemIndex = UKismetMathLibrary::RandomIntegerInRange(0, LootListArray.Num() - 1);
		if (LocalItemArray.Find(LocalItemIndex) == -1)
		{
			LootListTemp = LootListArray[LocalItemIndex];
			if (LootListTemp.DropChance >= UKismetMathLibrary::RandomFloatInRange(0.01f, 1.f))
			{
				LocalItemArray.AddUnique(LocalItemIndex);
				if (GetDataTableRowByName(DataTable, LootListTemp.ID, InventoryItemTemp))
				{
					/*UE_LOG(LogTemp, Warning, TEXT("Successfully retrieved InventoryItemTemp for ID: %s"), *InventoryItemTemp.ID.ToString());
					UE_LOG(LogTemp, Warning, TEXT("Successfully retrieved InventoryItemTemp for MaxStackSize: %d"), InventoryItemTemp.MaxStackSize);*/
					LocalItemAmount = UKismetMathLibrary::RandomIntegerInRange(LootListTemp.MinAmount, LootListTemp.MaxAmount);
					if (LocalItemAmount > GetItemMaxStackSize(InventoryItemTemp))
					{
						if (!GetItemIsCurrency(InventoryItemTemp))
						{						
							//LocalItemAmount = GetItemMaxStackSize(InventoryItemTemp);
							LocalItemAmount = 1;
						}
					}
					InventoryItemArray.Add(SetItemAmount(InventoryItemTemp, LocalItemAmount));
					//UE_LOG(LogTemp, Warning, TEXT("Added item to InventoryItemArray: %s, Amount: %d"), *InventoryItemTemp.ID.ToString(), LocalItemAmount);
					LocalLootCount++;
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to retrieve InventoryItemTemp for ID: %s"), *LootListTemp.ID.ToString());
				}
			}
		}
	}
	return InventoryItemArray;
}

FInventoryItem ALootActor::SetItemAmount(FInventoryItem& InvItem, int32 Amount)
{
	InvItem.Amount = Amount;
	return InvItem;
}

int32 ALootActor::GetItemMaxStackSize(FInventoryItem InventoryItem)
{
	return InventoryItem.MaxStackSize;
}

bool ALootActor::GetItemIsCurrency(FInventoryItem InventoryItem)
{
	if (InventoryItem.ItemType == EItemType::Currency)
	{
		return true;
	}
	return false;
}
