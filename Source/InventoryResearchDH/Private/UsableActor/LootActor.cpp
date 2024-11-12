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
	TArray<FInventoryItem> RandomInventoryItemArray;
	RandomInventoryItemArray = GetRandomLootItems();
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
	int32 LocalLootAmount = 0;
	int32 LocalLootCount = 0;
	int32 LocalItemIndex = 0;
	TArray<int32> LocalItemArray;
	int32 LocalItemAmount = 0;
	
	LootListArray = GetLootList();
	while (LocalLootCount < LocalLootAmount)
	{
		LocalItemIndex = UKismetMathLibrary::RandomIntegerInRange(0, LootListArray.Num() - 1);
		if (LocalItemArray.Find(LocalItemIndex) == -1)
		{
			LootListTemp = LootListArray[LocalItemIndex];
			if (LootListTemp.DropChance >= UKismetMathLibrary::RandomFloatInRange(0.01f, 1.f))
			{
				LocalItemArray.AddUnique(LocalItemIndex);
				if (GetDataTableRowByName(DataTable, LootListTemp.ID, InventoryItemTemp))
				{
					LocalItemAmount = UKismetMathLibrary::RandomIntegerInRange(LootListTemp.MinAmount, LootListTemp.MaxAmount);
					if (LocalItemAmount > GetItemMaxStackSize(InventoryItemTemp))
					{
						if (!GetItemIsCurrency(InventoryItemTemp))
						{
							LocalItemAmount = GetItemMaxStackSize(InventoryItemTemp);
						}
					}
					InventoryItemArray.Add(SetItemAmount(InventoryItemTemp, LocalItemAmount));
					LocalLootCount = LocalLootCount + 1;
				}
			}
		}
	}
	return InventoryItemArray;
}

FInventoryItem ALootActor::SetItemAmount(FInventoryItem InvItem, int32 Amount)
{
	FInventoryItem InvItemTemp;
	InvItemTemp.Amount = InvItem.Amount;
	return InvItemTemp;
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
