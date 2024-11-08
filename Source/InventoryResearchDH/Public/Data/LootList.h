#pragma once

#include "CoreMinimal.h"
#include "LootList.generated.h"

USTRUCT(BlueprintType)
struct FLootList
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot List")
    FName ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot List")
    bool bUnique = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot List")
    int32 MinAmount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot List")
    int32 MaxAmount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot List")
    float DropChance = 0.f;
};

