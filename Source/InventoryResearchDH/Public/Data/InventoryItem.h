// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemType.h"
#include "EquipmentType.h"
#include "ItemQuality.h"
#include "EquipmentSlotType.h"
#include "InventoryItem.generated.h"

// Forward declarations
class UTexture2D;
class UStaticMesh;
class USkeletalMesh;

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
    GENERATED_BODY()

public:
    // ID of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ID;

    // Icon texture of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Icon;

    // Name of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName Name;

    // Description of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Description;

    // Quality of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemQuality Quality;

    // Type of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

    // Amount of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Amount = 1;

    // Whether the item is stackable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsStackable;

    // Maximum stack size of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxStackSize;

    // Whether the item is droppable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsDroppable;

    // Mesh for the item in the world
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UStaticMesh* WorldMesh;

    // Health provided by the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Health;

    // Duration of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float Duration;

    // Weapon actor class for the item
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    //TSubclassOf<AWeaponActor> WeaponActorClass;

    // Mesh for the item when equipped
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    USkeletalMesh* EquipmentMesh;

    // Equipment type of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EEquipmentType EquipmentType;

    // Slot where the item can be equipped
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EEquipmentSlotsType EquipmentSlot;

    // Damage value of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Damage;

    // Armor value of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Armor;

    // Strength value provided by the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Strength;

    // Dexterity value provided by the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Dexterity;

    // Intelligence value provided by the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Intelligence;
};
