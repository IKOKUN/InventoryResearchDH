#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "EquipmentSlotType.h"
#include "ItemQuality.h"
#include "ItemType.h"
#include "EquipmentType.h"
#include "ToolTipInfo.generated.h"

// Forward declaration
class UTexture2D;

USTRUCT(BlueprintType)
struct FToolTipInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    EItemQuality Quality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    bool bIsStackable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    int32 MaxStackSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    EEquipmentType EquipmentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    EEquipmentSlotsType EquipmentSlot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    int32 Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    int32 Armor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    int32 Strength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    int32 Dexterity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip Info")
    int32 Intelligence;

    // Constructor
    /*FToolTipInfo()
        : Icon(nullptr),
        Name(TEXT("")),
        Description(TEXT("")),
        Quality(EItemQuality::Common),
        ItemType(EItemType::Miscellaneous),
        bIsStackable(false),
        MaxStackSize(0),
        Health(0),
        Duration(0.0f),
        EquipmentType(EEquipmentType::Armor),
        EquipmentSlot(EEquipmentSlotType::Head),
        Damage(0),
        Armor(0),
        Strength(0),
        Dexterity(0),
        Intelligence(0)
    {}*/
};