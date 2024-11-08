// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Miscellaneous        UMETA(DisplayName = "Miscellaneous"),
    Equipment    UMETA(DisplayName = "Equipment"),
    Consumable       UMETA(DisplayName = "Consumable"),
    Currency       UMETA(DisplayName = "Currency")
};
