// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemQuality.generated.h"

UENUM(BlueprintType)
enum class EItemQuality : uint8
{
    Poor        UMETA(DisplayName = "Poor"),
    Common    UMETA(DisplayName = "Common"),
    Uncommon       UMETA(DisplayName = "Uncommon"),
    Rare       UMETA(DisplayName = "Rare"),
    Epic        UMETA(DisplayName = "Epic"),
    Legendary        UMETA(DisplayName = "Legendary"),

	// Enumerator untuk menentukan jumlah total entri
	EItemQuality_MAX UMETA(Hidden)
};
