// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor/UsableActorBase.h"
#include "../Data/InventoryInterface.h"
#include "../Data/InventoryItem.h"
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
	bool LoadInventoryItems(int32 InvSize, TArray<FInventoryItem> InvItems);

	/* Implementing Inventory Interface */
	virtual FContainerProperties GetContainerProperties() override;
	virtual UInventoryComponent* GetContainerInvetory() override;
	virtual bool GetCanStoreItems() override;
	virtual bool ContainerLooted() override;
	/* End Implementing Inventory Interface */

	// Implementing IUsableActorInterface
	virtual bool OnActorUsed(APlayerController* PlayerController) override;
	// End Implementing IUsableActorInterface

	// Total Amount of Item can store
	int32 InventorySize = 4;

	// Total amount of item can store each row
	uint8 SlotsPerRow = 4;

	// Is Actor Can Store An Items
	bool bCanStoreItem;
};
