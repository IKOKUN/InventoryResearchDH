// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSockets.generated.h"


USTRUCT(BlueprintType)
struct FEquipmentSockets
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
    FName RightRing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
    FName LeftRing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
    FName MainHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
    FName OffHand;

    FEquipmentSockets()
        : RightRing(NAME_None)
        , LeftRing(NAME_None)
        , MainHand(NAME_None)
        , OffHand(NAME_None)
    {}
};
