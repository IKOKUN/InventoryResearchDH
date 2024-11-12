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
        : Head(TEXT("None")),
        Shoulders(TEXT("None")),
        Chest(TEXT("None")),
        Hands(TEXT("None")),
        Legs(TEXT("None")),
        Feet(TEXT("None")),
        Accessory(TEXT("None")),
        Back(TEXT("None")),
        RightRing(TEXT("None")),
        LeftRing(TEXT("None")),
        Waist(TEXT("None")),
        Trinket(TEXT("None")),
        MainHand(TEXT("None")),
        OffHand(TEXT("None"))
    {
        // Initialize Inventory with one FInventoryItem with ItemID as "None" and Amount as 1
        Inventory.Add(FItem(TEXT("None"), 1));
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString Head;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString Shoulders;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString Chest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString Hands;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString Legs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString Feet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString Accessory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString Back;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString RightRing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString LeftRing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString Waist;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString Trinket;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString MainHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    FString OffHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Items")
    TArray<FItem> Inventory;
};