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
    FName ID = FName("None");

    // Icon texture of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Icon = nullptr;

    // Name of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName Name = FName("None");

    // Description of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Description = FString(TEXT(""));

    // Quality of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemQuality Quality = EItemQuality::Common;

    // Type of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType = EItemType::Miscellaneous;

    // Amount of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Amount = 1;

    // Whether the item is stackable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsStackable = false;

    // Maximum stack size of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxStackSize = 0;

    // Whether the item is droppable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsDroppable = false;

    // Mesh for the item in the world
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UStaticMesh* WorldMesh = nullptr;

    // Health provided by the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float Health = 0.0f;

    // Duration of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float Duration = 0.0f;

    // Weapon actor class for the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSubclassOf<AActor> WeaponActorClass;

    // Mesh for the item when equipped
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    USkeletalMesh* EquipmentMesh = nullptr;

    // Equipment type of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EEquipmentType EquipmentType = EEquipmentType::Armor;

    // Slot where the item can be equipped
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EEquipmentSlotsType EquipmentSlot = EEquipmentSlotsType::Head;

    // Damage value of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Damage = 0;

    // Armor value of the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Armor = 0;

    // Strength value provided by the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Strength = 0;

    // Dexterity value provided by the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Dexterity = 0;

    // Intelligence value provided by the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Intelligence = 0;

    // Default constructor
    FInventoryItem()
        : ID(NAME_None),
        Icon(nullptr),
        Name(NAME_None),
        Description(TEXT("")),
        Quality(EItemQuality::Common), // Set default quality
        ItemType(EItemType::Miscellaneous), // Set default item type
        Amount(1),
        bIsStackable(false),
        MaxStackSize(0),
        bIsDroppable(true),
        WorldMesh(nullptr),
        Health(0.0f),
        Duration(0.0f),
        WeaponActorClass(nullptr),
        EquipmentMesh(nullptr),
        EquipmentType(EEquipmentType::Armor), // Set default equipment type
        EquipmentSlot(EEquipmentSlotsType::Head), // Set default equipment slot
        Damage(0),
        Armor(0),
        Strength(0),
        Dexterity(0),
        Intelligence(0)
    {
    }
};
