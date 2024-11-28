// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Data/InventoryItem.h"
#include "InventoryHUDInterface.generated.h"


USTRUCT(BlueprintType)
struct FPlayerStats
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Gold = 0; // Inisialisasi dengan 0

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Damage = 0; // Inisialisasi dengan 0

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Armor = 0; // Inisialisasi dengan 0

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Strength = 0; // Inisialisasi dengan 0

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Dexterity = 0; // Inisialisasi dengan 0

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Intelligence = 0; // Inisialisasi dengan 0

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 InventorySize = 0; // Inisialisasi dengan 0
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYRESEARCHDH_API IInventoryHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    // Fungsi-fungsi yang ada pada widget di gambar
    virtual void UI_Close_Inventory() = 0;
    virtual void UI_Close_Equipment() = 0;
    virtual void UI_Close_Container() = 0;
    virtual void UI_Set_IsMouseOverUI(bool bIsMouseOver) = 0;
    virtual FPlayerStats UI_Get_PlayerStats() = 0;
    virtual bool UI_Get_IsShiftKeyDown() = 0;
    virtual void UI_Equip_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot) = 0;
    virtual void UI_UnEquip_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot) = 0;
    virtual void UI_Split_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot, int32 Amount) = 0;
    virtual void UI_Move_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot) = 0;
    virtual void UI_Split_Container_Item(int32 FromContainerSlot, int32 ToContainerSlot, int32 Amount) = 0;
    virtual void UI_Move_Container_Item(int32 FromContainerSlot, int32 ToContainerSlot) = 0;
    virtual void UI_Take_Container_Item(int32 FromContainerSlot, int32 ToContainerSlot) = 0;
    virtual void UI_Deposit_Container_Item(int32 FromInvSlot, int32 ToContainerSlot) = 0;
    virtual void UI_Split_Item_From_Inventory(int32 FromInventorySlot, int32 ToSlot, int32 Amount) = 0;
	virtual void UI_Split_Item_From_Container(int32 FromContainerSlot, int32 ToSlot, int32 Amount) = 0;
    virtual void UI_Use_Inventory_Item(int32 InvSlot) = 0;
    virtual void UI_Use_Container_Item(int32 ContainerSlot) = 0;
    virtual void UI_Move_Hotbar_Item(int32 FromSlot, int32 ToSlot, bool bIsDraggedFromInventory, bool bIsDraggedFromHotbar) = 0;
    virtual void UI_Drop_Inventory_Item(int32 FromInvSlot) = 0;
    virtual void UI_Clear_Hotbar_Item(int32 FromHotbarSlot) = 0;
    virtual void UI_Equip_From_Container(int32 FromContainerSlot, int32 ToInvSlot) = 0;
    virtual void UI_UnEquip_To_Container(int32 FromInvSlot, int32 ToContainerSlot) = 0;
    virtual FInventoryItem UI_Get_ToolTip_Info(FName ItemID) = 0;
};
