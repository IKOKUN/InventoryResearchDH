// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent/InventoryComponent.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UInventoryComponent::InitInventory(int32 InventorySize)
{
	InventoryItems.Empty();
	for (int32 i = 0; i < InventorySize - 1; i++)
	{
		InventoryItems.Add(FInventoryItem());
	}
	return true;
}

FInventoryItem UInventoryComponent::GetInventoryItem(int32 InvSlot)
{
	if (InvSlot >= 0 && InvSlot < InventoryItems.Num()) // Pastikan slot valid
	{
		UE_LOG(LogTemp, Log, TEXT("Getting item at slot %d: ID: %s"), InvSlot, *InventoryItems[InvSlot].ID.ToString());

		if (ItemIsValid(InventoryItems[InvSlot]))
		{
			// Log jika item valid
			UE_LOG(LogTemp, Log, TEXT("Item at slot %d is valid: ID: %s, Amount: %d"), InvSlot, *InventoryItems[InvSlot].ID.ToString(), InventoryItems[InvSlot].Amount);
			return InventoryItems[InvSlot];
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GetInventoryItem: Failed to get item at slot %d."), InvSlot);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetInventoryItem: Invalid slot index %d."), InvSlot);
	}

	return FInventoryItem(); // Kembalikan item default jika tidak valid
}



bool UInventoryComponent::SetInventoryItem(int32 InvSlot, FInventoryItem Item)
{
	if (SetInventoryArrayElement(InventoryItems, InvSlot, Item, true))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SetInventoryItem: Failed to set item at slot %d."), InvSlot);
		return false;
	}
}

bool UInventoryComponent::ClearInventoryItem(int32 InvSlotIndex)
{
	SetInventoryArrayElement(InventoryItems, InvSlotIndex, FInventoryItem(), false);
	return true;
}

bool UInventoryComponent::GetEmptyInventorySpace(int32& OutIndex)
{
	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
		if (!ItemIsValid(InventoryItems[OutIndex]))
		{
			OutIndex = i;
			return true;
		}
	}
	return false;
}

int32 UInventoryComponent::GetInventoryItemCount()
{
	int32 LocalItemCount = 0;
	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
		if (ItemIsValid(InventoryItems[i]))
		{
			LocalItemCount++;
		}
	}

	return LocalItemCount;
}

bool UInventoryComponent::IncreaseInventorySize(int32 Amount)
{
	for (int32 i = 0; i < Amount - 1; i++)
	{
		InventoryItems.Add(FInventoryItem());
	}
	return false;
}

bool UInventoryComponent::DecreaseInventorySize(int32 Amount)
{
	TArray<int32>LocAmountIndexes;
	int32 LocFirstIndex = InventoryItems.Num() - Amount;
	int32 LocLastIndex = InventoryItems.Num() - 1;
	for (int32  i = LocFirstIndex;  i < LocLastIndex; i++)
	{
		LocAmountIndexes.Add(i);
	}
	for (int32 i = 0; i < LocAmountIndexes.Num() - 1; i++)
	{
		int32 LocIndex = LocAmountIndexes.Num() - 1 - i;
		InventoryItems.RemoveAt(LocIndex);
	}
	return true;
}

bool UInventoryComponent::LoadInventoryItems(int32 InventorySize, TArray<FInventoryItem> InvItems)
{
	InventoryItems.Empty();
	for (int32 i = 0; i < InventorySize - 1; i++)
	{
		InventoryItems.Add(FInventoryItem());
	}
	for (int32 j = 0; j < InvItems.Num() - 1; j++)
	{
		SetInventoryItem(j, InventoryItems[j]);
	}
	return true;
}

TArray<FInventoryItem> UInventoryComponent::GetInventoryItems()
{
	return InventoryItems;
}

bool UInventoryComponent::ItemIsValid(FInventoryItem InvItem)
{
	return InvItem.Icon != nullptr;
}

bool UInventoryComponent::SetInventoryArrayElement(TArray<FInventoryItem>& TargetArray, int32 Index, const FInventoryItem& Item, bool bSizeToFit)
{
	// Validasi parameter Index
	if (Index < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SetInventoryArrayElement: Index %d is invalid. Index must be non-negative."), Index);
		return false;
	}

	// Menyesuaikan ukuran array jika diperlukan
	if (bSizeToFit && Index >= TargetArray.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInventoryArrayElement: Resizing array to fit Index %d (current size: %d)."), Index, TargetArray.Num());
		TargetArray.SetNum(Index + 1);  // Perbesar array agar cukup untuk elemen baru
	}

	// Validasi ulang setelah resize
	if (!TargetArray.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("SetInventoryArrayElement: Failed to set element. Index %d is out of bounds (array size: %d)."), Index, TargetArray.Num());
		return false;
	}

	// Set elemen array
	TargetArray[Index] = Item;

    UE_LOG(LogTemp, Log, TEXT("SetInventoryArrayElement: Successfully set element at Index %d. ItemName: %s, Quantity: %d"), Index, *Item.ID.ToString(), Item.Amount);
	return true;
}



