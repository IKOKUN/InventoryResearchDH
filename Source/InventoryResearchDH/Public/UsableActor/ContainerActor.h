// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor/UsableActorBase.h"
#include "../Data/InventoryInterface.h"
#include "ContainerActor.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API AContainerActor : public AUsableActorBase, public IInventoryInterface
{
	GENERATED_BODY()

public:
	AContainerActor();

protected:
	virtual void BeginPlay() override;

	// Inventory Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	// function fo loading inventory
	void InitInventory();

	// Load Inventory Items
	void LoadInventoryItems();

	/* Implementing Inventory Interface */
	virtual FContainerProperties GetContainerProperties() override;
	virtual UInventoryComponent* GetContainerInvetory() override;
	virtual bool GetCanStoreItems() override;
	virtual bool ContainerLooted() override;
	//virtual bool AddItemToInventory(FItemDetails Item) override;
	//virtual bool RemoveItemFromInventory(FItemDetails Item) override;
	//virtual bool RemoveItemFromInventoryByIndex(int32 Index) override;
	/* End Implementing Inventory Interface */
};
