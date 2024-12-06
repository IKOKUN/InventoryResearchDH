// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor/UsableActorBase.h"
#include "Data/InventoryItem.h"
#include "WorldActor.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API AWorldActor : public AUsableActorBase
{
	GENERATED_BODY()
	
public:
	AWorldActor();

	bool LoadItemFromList();
	void UpdateItemAmount();
protected:
	virtual void BeginPlay() override;

	// IUsableActorInterface
	virtual bool OnActorUsed(APlayerController* PlayerController) override;
private:

	UPROPERTY(EditAnywhere, Category = "Properties")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Properties")
	int32 Amount;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FInventoryItem InventoryItem;

public:
	// Get ID
	FORCEINLINE FName GetID() const { return ID; }
	// Get Amount
	FORCEINLINE int32 GetAmount() const { return Amount; }
	// Get Inventory Item
	FORCEINLINE FInventoryItem GetInventoryItem() const { return InventoryItem; }
	// Set Inventory Item
	FORCEINLINE void SetInventoryItem(FInventoryItem Item) { InventoryItem = Item; }
	// Set Amount
	FORCEINLINE void SetAmount(int32 NewAmount) { Amount = NewAmount; }
	// Set ID
	FORCEINLINE void SetID(FName NewID) { ID = NewID; }
};
