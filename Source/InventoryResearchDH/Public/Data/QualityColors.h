#pragma once

#include "CoreMinimal.h"

#include "QualityColors.generated.h"

USTRUCT(BlueprintType)
struct FQualityColors
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Colors")
    FLinearColor Poor = FLinearColor(0.3f, 0.3f, 0.3f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Colors")
    FLinearColor Common = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Colors")
    FLinearColor Uncommon = FLinearColor(0.12f, 1.0f, 0.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Colors")
    FLinearColor Rare = FLinearColor(0.0f, 0.44f, 0.87f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Colors")
    FLinearColor Epic = FLinearColor(0.64f, 0.21f, 0.93f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Colors")
    FLinearColor Legendary = FLinearColor(1.0f, 0.5f, 0.0f, 1.0f);
};
