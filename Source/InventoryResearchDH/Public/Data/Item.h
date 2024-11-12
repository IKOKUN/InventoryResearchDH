#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"

// Forward declaration
class UTexture2D;

USTRUCT(BlueprintType)
struct FItem
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Amount;

    // Parameterized constructor
    struct FItem(FName InItemID = "None", int32 InAmount = 1)
        : ItemID(InItemID), Amount(InAmount) {}
};
