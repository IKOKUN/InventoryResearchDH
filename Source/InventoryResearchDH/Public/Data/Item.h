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
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID;

};
