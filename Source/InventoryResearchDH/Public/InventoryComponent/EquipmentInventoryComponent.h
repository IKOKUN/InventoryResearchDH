// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent/InventoryComponent.h"
#include "../Data/EquipmentSlots.h"
#include "EquipmentInventoryComponent.generated.h"

class AIRCharacter;
class USkeletalMesh;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UEquipmentInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
	
public:
	UEquipmentInventoryComponent();

	// Initializes The Inventory Array To A Specified Size
	virtual bool InitInventory(int32 InventorySize) override;

	// Sets The Inventory Item In The Inventory
	virtual bool SetInventoryItem(int32 InvSlot, FInventoryItem Item) override;

	// Returns The First Available Inventory Space
	virtual bool ClearInventoryItem(int32 InvSlotIndex) override;

	// Returns The First Available Inventory Space
	virtual bool GetEmptyInventorySpace(int32& OutIndex) override;

	// Clears The Inventory and Fills It With The Loaded Items
	virtual bool LoadInventoryItems(int32 InventorySize, TArray<FInventoryItem> InvItems) override;
	
	void UpdateEquippedMeshes(int32 InvSlot);
	void UpdateMainHandWeapon(TSubclassOf<AActor> WeaponActorClass, USkeletalMesh* EquipmentMesh);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Properties | Character")
	TObjectPtr<AIRCharacter> PlayerCharacter;
public:
	// Fungsi untuk mendapatkan karakter pemain
	FORCEINLINE void SetPlayerCharacter(AIRCharacter* Character) {
		PlayerCharacter = Character;
		//PlayerCharacter = Character;
	}

	// Fungsi untuk mendapatkan jumlah entri dalam EEquipmentSlots
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helper")
	FORCEINLINE int32 GetNumberOfEquipmentSlots() const {
		return static_cast<int32>(EEquipmentSlots::EEquipmentSlots_MAX);
	}
};

