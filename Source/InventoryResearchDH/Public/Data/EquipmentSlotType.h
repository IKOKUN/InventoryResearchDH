// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSlotType.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlotsType : uint8
{
    Head        UMETA(DisplayName = "Head"),
    Shoulder    UMETA(DisplayName = "Shoulder"),
    Chest       UMETA(DisplayName = "Chest"),
    Hands       UMETA(DisplayName = "Hands"),
    Legs        UMETA(DisplayName = "Legs"),
    Feet        UMETA(DisplayName = "Feet"),
    Back        UMETA(DisplayName = "Back"),
    Waist       UMETA(DisplayName = "Waist"),
    Accessory   UMETA(DisplayName = "Accessory"),
    Earring     UMETA(DisplayName = "Earring"),
    Ring        UMETA(DisplayName = "Ring"),
    Trinket     UMETA(DisplayName = "Trinket"),
    MainHand    UMETA(DisplayName = "Main-Hand"),
    OffHand     UMETA(DisplayName = "Off-Hand")
};