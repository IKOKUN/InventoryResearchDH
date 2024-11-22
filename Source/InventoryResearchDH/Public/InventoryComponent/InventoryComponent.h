// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Data/InventoryItem.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYRESEARCHDH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	// Initializes The Inventory Array To A Specified Size
	virtual bool InitInventory(int32 InventorySize);
	
	// Returns The Inventory Item From The Inventory
	FInventoryItem GetInventoryItem(int32 InvSlot);
	
	// Sets The Inventory Item In The Inventory
	virtual bool SetInventoryItem(int32 InvSlot, FInventoryItem Item);
	
	// Returns The First Available Inventory Space
	virtual bool ClearInventoryItem(int32 InvSlotIndex);
	
	// Returns The First Available Inventory Space
	virtual bool GetEmptyInventorySpace(int32& OutIndex);
	
	// Returns The Count Of Valid Inventory Items
	int32 GetInventoryItemCount();
	
	// Adds New Amount To Existing Inventory
	bool IncreaseInventorySize(int32 Amount);
	
	// Removes Amount From The End Of The Array
	bool DecreaseInventorySize(int32 Amount);
	
	// Clears The Inventory and Fills It With The Loaded Items
	virtual bool LoadInventoryItems(int32 InventorySize, TArray<FInventoryItem> InvItems);

	//Returns An Array Of The Inventory Items
	TArray<FInventoryItem> GetInventoryItems();

	//Check If Item Is Valid
	bool ItemIsValid(FInventoryItem InvItem);

	//Set Inventory Array Element
	bool SetInventoryArrayElement(TArray<FInventoryItem>& TargetArray, int32 Index, const FInventoryItem& Item, bool bSizeToFit);
protected:
	virtual void BeginPlay() override;

	// Inventory Array
	TArray<FInventoryItem> InventoryItems;

public:
	FORCEINLINE TArray<FInventoryItem> GetInventoryItems() const { return InventoryItems; }
};
