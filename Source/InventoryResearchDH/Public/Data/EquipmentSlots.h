// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSlots.generated.h"


UENUM(BlueprintType)
enum class EEquipmentSlots : uint8
{
    Head        UMETA(DisplayName = "Head"),
    Shoulder    UMETA(DisplayName = "Shoulder"),
    Chest       UMETA(DisplayName = "Chest"),
    Hands       UMETA(DisplayName = "Hands"),
    Legs        UMETA(DisplayName = "Legs"),
    Feet        UMETA(DisplayName = "Feet"),
    Accessory        UMETA(DisplayName = "Accessory"),
    Back       UMETA(DisplayName = "Back"),
    RightRing   UMETA(DisplayName = "RightRing"),
    LeftRing     UMETA(DisplayName = "LeftRing"),
    Waist        UMETA(DisplayName = "Waist"),
    Trinket     UMETA(DisplayName = "Trinket"),
    MainHand    UMETA(DisplayName = "Main-Hand"),
    OffHand     UMETA(DisplayName = "Off-Hand")
};
