// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContainerInfo.generated.h"

/ USTRUCT(BlueprintType)
struct FContainerInfo
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
    FName Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
    uint8 SlotsPerRow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
    bool bIsStorageContainer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
    int32 StorageInventorySize;

    FContainerInfo()
        : Name(NAME_None)
        , SlotsPerRow(0)
        , bIsStorageContainer(false)
        , StorageInventorySize(10)
    {}
};
