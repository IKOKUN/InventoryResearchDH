// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Interfaces/InventoryHUDInterface.h"
#include "../Data/NPCItems.h"
#include "../Data/InventoryItem.h"
#include "IRPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UHUDLayoutWidget;
class AUsableActorBase;
class AIRCharacter;
class AIRHUD;
class UInteractSlotWidget;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EClassesHero : uint8
{
	Warrior UMETA(DisplayName = "Warrior"),
	Rogue UMETA(DisplayName = "Rogue"),
	Wizard UMETA(DisplayName = "Wizard")
};

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
	virtual void UI_Split_Container_Item(int32 FromContainerSlot, int32 ToContainerSlot, int32 Amount) override;
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

	/* Regen Health */
	bool SetHealth(int32 Amount, float Duration);
	void RegenHealth();
	void StartRegenHealth();
	void StopRegenHealth();
	
	/* Regen Health */

	/* Interaction */
	void SetInteractText(FText InputText, bool bCanInspect = true);
	FText GetActorActionText(AUsableActorBase* UsableActor);
	void ShowInteractText();
	void HideInteractText();


	AUsableActorBase* GetUsableActor();
	void OnActorUsed();
	void GetUsableActorFocus();

	void InitializePlayer();

	void LoadPlayerItems();


	bool GetDataTableRowByName(UDataTable* SrcDataTable, const FName RowName, FNPCItems& OutNPCInvItemRow);
	bool GetDataTableRowByName(UDataTable* SrcDataTable, const FName RowName, FInventoryItem& OutNPCInvItemRow);

	UPROPERTY(EditAnywhere, Category = "Properties | Data Table")
	TObjectPtr<UDataTable> ClassStartingEquipment;

	UPROPERTY(EditAnywhere, Category = "Properties | Data Table")
	TObjectPtr<UDataTable> ItemListDataTable;

	FName EnumToFName(EClassesHero EnumValue);
	FName EnumToFNameByValue(EClassesHero EnumValue);

	// Interact Slot Widget
	TObjectPtr<UInteractSlotWidget> ActiveInteractSlotWidget;
protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	/* Input Function */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJumping();

	void SteerCharacter(const FInputActionValue& Value);
	void OpenEquipmentAndInventory();
	void OpenEquipment();
	void OpenInventory();
	void UseActor();
	void InspectActor();
	void Hotbar1();
	void Hotbar2();
	void Hotbar3();
	void Hotbar4();
	void Hotbar5();
	void Hotbar6();
	void Hotbar7();
	void Hotbar8();
	/* End  Input Function*/

	/* Player Inventory Component for Handling Player Inventory*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UEquipmentInventoryComponent> PlayerInventoryComponent;

	/* Inventory Manager Component for connect Player Controller with UI*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UInventoryManagerComponent> InventoryManagerComponent;

	UPROPERTY(EditAnywhere, Category = "Ref | Character")
	TObjectPtr<AIRCharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = "Ref | HUD")
	TObjectPtr<AIRHUD> PlayerHUD;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SteerCharacterAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> OpenEquipmentAndInventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> OpenEquipmentAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> OpenInventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UseActorAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InspectActorAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar4Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar5Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar6Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar7Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar8Action;
	/* End Input */

	/* Character Properties */
	UPROPERTY(EditAnywhere, Category = "Properties | Character")
	FName Name = FName(TEXT("Player"));

	UPROPERTY(EditAnywhere, Category = "Properties | Character")
	float Health = 60.f;

	UPROPERTY(EditAnywhere, Category = "Properties | Character")
	float MaxHealth = 100.0f;
	/* Character Properties */

	/* Interaction Properties */
	UPROPERTY(EditAnywhere, Category = "Properties | Interaction")
	bool bIsNewFocus;

	UPROPERTY(EditAnywhere, Category = "Properties | Interaction")
	TObjectPtr<AUsableActorBase> LastUsableActor;

	UPROPERTY(EditAnywhere, Category = "Properties | Interaction")
	float MaxUseDistance = 200.f;
	/* Interaction Properties */

	/* Consumable Properties */
	UPROPERTY(EditAnywhere, Category = "Properties | Consumable")
	float HealthTick = 0.f;

	UPROPERTY(EditAnywhere, Category = "Properties | Consumable")
	int32 RegenTickCount = 0;

	UPROPERTY(EditAnywhere, Category = "Properties | Consumable")
	int32 RegenDuration = 0;

	UPROPERTY(EditAnywhere, Category = "Properties | Consumable")
	bool bIsPoisoned = false;
	/* Consumable Properties */

	/* Input Properties */
	UPROPERTY(EditAnywhere, Category = "Properties | Input")
	bool bIsShiftKeyDown = false;

	UPROPERTY(EditAnywhere, Category = "Properties | Input")
	bool bIsMovementLocked = false;

	UPROPERTY(EditAnywhere, Category = "Properties | Input")
	bool bIsMouseOverUI = false;

	UPROPERTY(EditAnywhere, Category = "Properties | Input")
	bool bIsRightMouseButtonDown = false;

	UPROPERTY(EditAnywhere, Category = "Properties | Input")
	bool bOnInspectObject = false;
	/* Input Properties */

	/* HUD Properties */
	UPROPERTY(EditAnywhere, Category = "Properties | HUD")
	TObjectPtr<UHUDLayoutWidget> HUDReference;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UHUDLayoutWidget> MainHUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bIsThirdPersonView = false;


	// Timer handle for health regeneration
	FTimerHandle RegenTimerHandle;


public:
	FORCEINLINE UEquipmentInventoryComponent* GetPlayerInventoryComponent() const { return PlayerInventoryComponent.Get(); }
	FORCEINLINE UInventoryManagerComponent* GetInventoryManagerComponent() const { return InventoryManagerComponent.Get(); }
	FORCEINLINE FName GetPlayerName() const { return Name; }
	FORCEINLINE float GetPlayerHealth() const { return Health; }
	FORCEINLINE float GetPlayerMaxHealth() const { return MaxHealth; }

	// Fungsi untuk mengubah elemen dalam array dengan menyesuaikan ukuran array jika perlu
	UFUNCTION(BlueprintCallable, Category = "Array Operations")
	bool SetInventoryArrayElement(TArray<FInventoryItem>& TargetArray, int32 Index, const FInventoryItem& Item, bool bSizeToFit)
	{
		// Menyesuaikan ukuran array jika diperlukan
		if (bSizeToFit && Index >= TargetArray.Num())
		{
			TargetArray.SetNum(Index + 1);  // Perbesar array agar cukup untuk elemen baru
		}

		// Cek apakah array sudah cukup besar untuk indeks tersebut
		if (TargetArray.IsValidIndex(Index))
		{
			TargetArray[Index] = Item;
			return true;
		}

		return false;  // Kembalikan false jika indeks tidak valid dan tidak ada perubahan
	}
};
