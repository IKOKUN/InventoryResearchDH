#pragma once

#include "CoreMinimal.h"
#include "ItemType.h"
#include "ItemQuality.h"
#include "ItemInformation.generated.h"

// Forward declaration
class UTexture2D;

USTRUCT(BlueprintType)
struct FItemInformation
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    FName ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    EItemQuality Quality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    EItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    int32 Amount = 1;
};

