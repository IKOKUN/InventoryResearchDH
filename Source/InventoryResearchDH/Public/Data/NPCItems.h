// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Engine/DataTable.h"
#include "NPCItems.generated.h"

// Main struct representing NPC items
USTRUCT(BlueprintType)
struct FNPCItems : public FTableRowBase
{
    GENERATED_BODY()

    // Default constructor
    FNPCItems()
        : Head("None"),
        Shoulders("None"),
        Chest("None"),
        Hands("None"),
        Legs("None"),
        Feet("None"),
        Accessory("None"),
        Back("None"),
        RightRing("None"),
        LeftRing("None"),
        Waist("None"),
        Trinket("None"),
        MainHand("None"),
        OffHand("None")
    {
        // Initialize Inventory with one FInventoryItem with ItemID as "None" and Amount as 1
        Inventory.Add(FItem("None", 1));
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName Head;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName Shoulders;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName Chest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName Hands;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName Legs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName Feet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName Accessory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName Back;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName RightRing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName LeftRing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName Waist;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName Trinket;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName MainHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FName OffHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    TArray<FItem> Inventory;
};