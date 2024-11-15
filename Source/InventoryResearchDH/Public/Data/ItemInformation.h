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
    // Konstruktor default
    FItemInformation()
        : ID(NAME_None)
        , Icon(nullptr)
        , Name(TEXT("None"))
        , Quality(EItemQuality::Common) // atau nilai default lainnya
        , Type(EItemType::Miscellaneous)      // atau nilai default lainnya
        , Amount(1)                    // Nilai default Amount adalah 1
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    FName ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    FName Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    EItemQuality Quality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    EItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
    int32 Amount;
};

