// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EquipmentLoadout.generated.h"

// Struct representing an equipment loadout
USTRUCT(BlueprintType)
struct FEquipmentLoadout : public FTableRowBase
{
    GENERATED_BODY()

    // Default constructor
    FEquipmentLoadout()
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
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName Head;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName Shoulders;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName Chest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName Hands;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName Legs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName Feet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName Accessory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName Back;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName RightRing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName LeftRing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName Waist;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName Trinket;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName MainHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Loadout")
    FName OffHand;
};


