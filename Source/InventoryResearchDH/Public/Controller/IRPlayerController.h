// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Interfaces/InventoryHUDInterface.h"
#include "IRPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API AIRPlayerController : public APlayerController, public IInventoryHUDInterface
{
	GENERATED_BODY()

public:
	AIRPlayerController();

	/* IInventoryHUDInterface */
	/* Implementasi fungsi-fungsi yang ada pada widget di gambar */
	virtual void UI_Close_Inventory() override;
	virtual void UI_Close_Equipment() override;
	virtual void UI_Close_Container() override;
	virtual void UI_Set_IsMouseOverUI(bool bIsMouseOver) override;
	virtual FPlayerStats UI_Get_PlayerStats() override;
	virtual bool UI_Get_IsShiftKeyDown() override;
	virtual void UI_Equip_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot) override;
	virtual void UI_UnEquip_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot) override;
	virtual void UI_Split_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot, int32 Amount) override;
	virtual void UI_Move_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot) override;
	virtual void UI_Split_Container_Item(int32 FromContainerSlot, int32 ToContainerSlot) override;
	virtual void UI_Move_Container_Item(int32 FromContainerSlot, int32 ToContainerSlot) override;
	virtual void UI_Take_Container_Item(int32 FromContainerSlot, int32 ToContainerSlot) override;
	virtual void UI_Deposit_Container_Item(int32 FromInvSlot, int32 ToContainerSlot) override;
	virtual void UI_Split_Item_From_Inventory(int32 FromInventorySlot, int32 ToSlot, int32 Amount) override;
	virtual void UI_Split_Item_From_Container(int32 FromContainerSlot, int32 ToSlot, int32 Amount) override;
	virtual void UI_Use_Inventory_Item(int32 InvSlot) override;
	virtual void UI_Use_Container_Item(int32 ContainerSlot) override;
	virtual void UI_Move_Hotbar_Item(int32 FromSlot, int32 ToSlot, bool bIsDraggedFromInventory, bool bIsDraggedFromHotbar) override;
	virtual void UI_Drop_Inventory_Item(int32 FromInvSlot) override;
	virtual void UI_Clear_Hotbar_Item(int32 FromHotbarSlot) override;
	virtual void UI_Equip_From_Container(int32 FromContainerSlot, int32 ToInvSlot) override;
	virtual void UI_UnEquip_To_Container(int32 FromInvSlot, int32 ToContainerSlot) override;
	virtual FInventoryItem UI_Get_ToolTip_Info(FName ItemID) override;
	/* End InventoryHUDInterface*/
protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	/* Base Movement Input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJumping();
	/* End Base Movement Input */

	/* Player Inventory Component for Handling Player Inventory*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UEquipmentInventoryComponent> PlayerInventoryComponent;

	/* Inventory Manager Component for connect Player Controller with UI*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UInventoryManagerComponent> InventoryManagerComponent;
private:
	/* Input */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_InventoryResearch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	/* End Input */

	/* Character Properties */
	UPROPERTY(EditAnywhere, Category = "Character")
	FName Name = FName(TEXT("Player"));

	UPROPERTY(EditAnywhere, Category = "Character")
	float Health = 60.f;

	UPROPERTY(EditAnywhere, Category = "Character")
	float MaxHealth = 100.0f;
	/* Character Properties */
public:
	FORCEINLINE UEquipmentInventoryComponent* GetPlayerInventoryComponent() const { return PlayerInventoryComponent.Get(); }
	FORCEINLINE UInventoryManagerComponent* GetInventoryManagerComponent() const { return InventoryManagerComponent.Get(); }
	FORCEINLINE FName GetPlayerName() const { return Name; }
	FORCEINLINE float GetPlayerHealth() const { return Health; }
	FORCEINLINE float GetPlayerMaxHealth() const { return MaxHealth; }
};
