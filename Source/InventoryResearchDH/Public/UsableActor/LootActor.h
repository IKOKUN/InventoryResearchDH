// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor/ContainerActor.h"
#include "../Data/InventoryItem.h"
#include "../Data/LootList.h"
#include "Engine/DataTable.h"
#include "LootActor.generated.h"


/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API ALootActor : public AContainerActor
{
	GENERATED_BODY()
	
public:
	ALootActor();

	UFUNCTION(BlueprintCallable, Category = "Data Table")
		TArray<FName> GetDataTableRowNames(TSoftObjectPtr<UDataTable> SrcDataTable);

	// Load Item From Inventory
	virtual bool InitInventory() override;

protected:
	virtual void BeginPlay() override;

	// Get List Avaliable Loot From This Actor
	TArray<FLootList> GetLootList();

	// Generate Random Loot Item
	TArray<FInventoryItem> GetRandomLootItems();

	// Set Total Item Amount From This Actor
	FInventoryItem SetItemAmount(FInventoryItem& InvItem, int32 Amount);

	// Minimal amount of Loot Items
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int32 MinLootItems;

	// Maximal amount of Loot Items
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int32 MaxLootItems;

	UPROPERTY(EditAnywhere, Category = "Data Table")
	TSoftObjectPtr<UDataTable> DataTableLootCommon;

public:
	// Get Item Max Size From Inventory 
	int32 GetItemMaxStackSize(FInventoryItem InventoryItem);
	// Check if item is Currency
	bool GetItemIsCurrency(FInventoryItem InventoryItem);
	
};
