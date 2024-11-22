// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentType.generated.h"

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
    Armor        UMETA(DisplayName = "Armor"),
    Weapon    UMETA(DisplayName = "Weapon"),
    Shield       UMETA(DisplayName = "Shield"),
    Miscellaneous       UMETA(DisplayName = "Miscellaneous"),

    // Enumerator untuk menentukan jumlah total entri
    EEquipmentType_MAX UMETA(Hidden)
};