// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent/InventoryComponent.h"
#include "../Data/InventoryItem.h"
#include "../Data/EquipmentSlotType.h"
#include "../Data/ItemInformation.h"
#include "../Data/ContainerInfo.h"
#include "InventoryManagerComponent.generated.h"

class UInventoryComponent;
class UUserWidget;
class UInventoryLayoutWidget;

USTRUCT(BlueprintType)
struct FWorldItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Item")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Item")
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Item")
	FName Name;

	FWorldItem() = default;
	FWorldItem(const FWorldItem&) = default;
	FWorldItem& operator=(const FWorldItem&) = default;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYRESEARCHDH_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryManagerComponent();
	
	/* Custom Event */
	UFUNCTION(BlueprintCallable, Category = "Event | Inventory Events")
	void InitInventoryItems();

	UFUNCTION(BlueprintCallable, Category = "Event | Inventory Events")
	void MoveInventoryItem(int32 FromInvSlot, int32 ToInvSlot);

	UFUNCTION(BlueprintCallable, Category = "Event | Inventory Events")
	void EquipFromInventory(int32 FromInvSlot, int32 ToInvSlot);

	UFUNCTION(BlueprintCallable, Category = "Event | Inventory Events")
	void UnequipFromInventory(int32 FromInvSlot, int32 ToInvSlot);

	UFUNCTION(BlueprintCallable, Category = "Event | Inventory Events")
	void DropItemFromInventory(int32 FromInvSlot);

	UFUNCTION(BlueprintCallable, Category = "Event | Inventory Events")
	void SplitInventoryItem(int32 FromInvSlot, int32 ToInvSlot, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Event | Inventory Events")
	void SplitItemFromInventory(int32 FromInvSlot, int32 ToInvSlot, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Event | Inventory Events")
	void EventIncreaseInventorySize(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Event | Inventory Events")
	void EventDecreaseInventorySize(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Event | Container Events")
	void MoveContainerItem(int32 FromContainerSlot, int32 ToContainerSlot);

	UFUNCTION(BlueprintCallable, Category = "Event | Container Events")
	void TakeContainerItem(int32 FromContainerSlot, int32 ToContainerSlot);

	UFUNCTION(BlueprintCallable, Category = "Event | Container Events")
	void DepositContainerItem(int32 FromInvSlot, int32 ToContainerSlot);

	UFUNCTION(BlueprintCallable, Category = "Event | Container Events")
	void EquipFromContainer(int32 FromContainerSlot, int32 ToInvSlot);

	UFUNCTION(BlueprintCallable, Category = "Event | Container Events")
	void UnEquipToContainer(int32 FromInvSlot, int32 ToContainerSlot);

	UFUNCTION(BlueprintCallable, Category = "Event | Container Events")
	void SplitContainerItem(int32 FromContainerSlot, int32 ToContainerSlot);

	UFUNCTION(BlueprintCallable, Category = "Event | Container Events")
	void SplitItemFromContainer(int32 FromContainerSlot, int32 ToSlot, int32 Amount);
	/* Custom Event */

	/* Function Of Manager */
	// Setup Components References
	UFUNCTION(BlueprintCallable, Category = "Manager | Public")
	void InitInventoryManager(UInventoryComponent* PlayerInventoryRef);

	// Setup Components UI References
	UFUNCTION(BlueprintCallable, Category = "Manager | Public")
	void InitInventoryManagerUI(UInventoryLayoutWidget* InventoryWidget);

	/*Tries to Add The Item To Inventory If there is free space, or available stack space.
		This must only be called by the authority.It is not an event because we may need to return an item.
		For an example of it's use, please look at the OnActorUse function in the Usable\World_Actor blueprint.*/
	UFUNCTION(BlueprintCallable, Category = "Manager | Public")
	bool TryAddItemToInventory(UInventoryComponent* Inventory, FInventoryItem& Item);

	// Remove the amount of an Item from the Inventory
	UFUNCTION(BlueprintCallable, Category = "Manager | Public")
	bool TryRemoveItemFromInventory(UInventoryComponent* Inventory, FName ItemID, int32 Amount);
	
	//Check to see if the inventory contains the amount of an item specified
	UFUNCTION(BlueprintCallable, Category = "Manager | Public")
	bool CheckInventoryForItemAmount(UInventoryComponent* Inventory, FName ItemID, int32 Amount);
	/* End Function Manager */


	/* Function Of Manager */
	// Helper : Returns The Equipment Type By Equip Slot Number
	UFUNCTION(BlueprintCallable, Category = "Manager")
	EEquipmentSlotsType GetEquipmentTypeBySlot(int32 EquipmentSlotIndex);

	// Helper: Returns The Equipment Slot By Equipment Type
	UFUNCTION(BlueprintCallable, Category = "Manager")
	int32 GetEquipmentSlotByType(EEquipmentSlotsType EqSlotType);

	// Returns The First Empty Equipment Slot By Equipment Type
	UFUNCTION(BlueprintCallable, Category = "Manager")
	bool GetEmptyEquipmentSlotByType(EEquipmentSlotsType EqSlotType, int32& OutEmptySlotIndex);

	// Checks If Current Container Can Store Items
	UFUNCTION(BlueprintCallable, Category = "Manager")
	bool CanContainerStoreItem(UInventoryComponent* Inventory);

	// Returns A Location At Random Rotation And Distance From The Character
	UFUNCTION(BlueprintCallable, Category = "Manager")
	FTransform RandomizeDropLocation();

	// When Picking Up A Stackable Item Try And Add It To Existing Stacks
	UFUNCTION(BlueprintCallable, Category = "Manager | Stacks")
	int32 FindAndAddAmountToStacks(UInventoryComponent* Inventory, FName ItemID, int32 Amount);

	//Add Amount To Inventory Stack And Return Remaining Amount
	UFUNCTION(BlueprintCallable, Category = "Manager | Stacks")
	int32 AddItemToStack(UInventoryComponent* Inventory, int32 InvSlot, int32 AmountToAdd);

	//Sets A New Item Amount(Amount Validation Should Be Done Before Calling Function)
	UFUNCTION(BlueprintCallable, Category = "Manager | Stacks")
	int32 SetItemAmount(UInventoryComponent* Inventory, int32 InvSlot, int32 AmountToAdd);

	// Adds Amount To Item Amount(Amount Validation Should Be Done Before Calling Function)
	UFUNCTION(BlueprintCallable, Category = "Manager | Stacks")
	int32 AddToItemAmount(FInventoryItem InvItem, int32 AmountToAdd);

	// Remove Amount From Item Amount
	UFUNCTION(BlueprintCallable, Category = "Manager | Stacks")
	bool RemoveFromItemAmount(FInventoryItem InvItem, int32& AmountToRemove);

	// Adds Item To Inventory Than Creates The UI Item Info And Sends To Clients
	// And If Player Inventory Updated Update HUD Inventory Slot Info
	// But If false Update HUD Inventory Slot Info 
	UFUNCTION(BlueprintCallable, Category = "Manager | Inventory")
	void AddItem(UInventoryComponent* InvComp, int32 InvSlot, FInventoryItem InvItem);

	// Add Item To Inventory Slot
	UFUNCTION(BlueprintCallable, Category = "Manager | Inventory")
	void RemoveItem(UInventoryComponent* InvComp, int32 InvSlot);

	// Handles Moving Items Between Inventories And Slots
	UFUNCTION(BlueprintCallable, Category = "Manager | Inventory")
	void MoveItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot);

	// Drops An Item Into The World Or Destroys It (If Not Droppable)
	UFUNCTION(BlueprintCallable, Category = "Manager | Inventory")
	void DropItem(UInventoryComponent* InvComp, int32 InvSlot);

	// Searches For Firs Specific Item In An Inventory
	UFUNCTION(BlueprintCallable, Category = "Manager | Inventory")
	bool HasItem(UInventoryComponent* InvComp, FName ItemID, int32& InvSlot);

	// Adds Gold To Player Stash
	UFUNCTION(BlueprintCallable, Category = "Manager | Inventory")
	void AddGold(int32 Amount);

	// Split Item Move (Similar To MoveItem But Handles Logic For Splitting A Stack To Create A New Item)
	UFUNCTION(BlueprintCallable, Category = "Manager | Inventory")
	void SplitItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot, int32 Amount);

	// Adds Amount To End of Inventory Array
	UFUNCTION(BlueprintCallable, Category = "Manager | Inventory")
	void IncreaseInventorySize(UInventoryComponent* InvComp, int32 Amount);

	// Removes Amount From End of player Inventory Array
	UFUNCTION(BlueprintCallable, Category = "Manager | Inventory")
	void DecreaseInventorySize(UInventoryComponent* InvComp, int32 Amount);

	// Checks To See If The Amount Of Slots At The End Of The Inventory That We Want To Remove Have Items In Them
	UFUNCTION(BlueprintCallable, Category = "Manager | Inventory")
	bool CheckSlotsForItems(UInventoryComponent* InvComp, int32 Amount);

	// Use Container (If Already Using It, Close It)
	UFUNCTION(BlueprintCallable, Category = "Manager | Container")
	void UseContainer(AActor* ContainerActor);

	// Get Container Inventory And Create/Send The Inventory Into To The Clients Container UI
	UFUNCTION(BlueprintCallable, Category = "Manager | Container")
	void OpenContainer(AActor* ContainerActor);

	// Close Container And Clear References
	UFUNCTION(BlueprintCallable, Category = "Manager | Container")
	void CloseContainer();

	// Use Container Item (Move Item From Container To Player Inventory)
	UFUNCTION(BlueprintCallable, Category = "Manager | Container")
	void UseContainerItem(int32 ContainerSlot);

	// Use Item In Inventory. Equips, Consumes Or Moves To A Container (If A Storage Container Is Open)
	UFUNCTION(BlueprintCallable, Category = "Manager | Items")
	void UseInventoryItem(int32 InvSlot);

	// Use Equippment Item.  Equips Or Unequips The Inventory Item When Used Or Moves To A Container (If A Storage Container Is Open)
	UFUNCTION(BlueprintCallable, Category = "Manager | Items")
	void UseEquipmentItem(int32 InvSlot, FInventoryItem InvItem, UInventoryComponent* ToInventory);

	// Use Consumbale Item. Consumes The Inventory Item When Used
	UFUNCTION(BlueprintCallable, Category = "Manager | Items")
	void UseConsumableItem(int32 InvSlot, FInventoryItem InvItem);

	// Tries To Equip An Item From Spesific Inventory
	UFUNCTION(BlueprintCallable, Category = "Manager | Equipment")
	void EquipItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot);

	// Tries To Unequip An Item From Specified Inventory
	UFUNCTION(BlueprintCallable, Category = "Manager | Equipment")
	void UnequipItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot);

	// Updates The Players Equipped Stats And Replicates The Changes
	UFUNCTION(BlueprintCallable, Category = "Manager | Equipment")
	void UpdateEquippedStats();
	/* End Function Manager */

	/* Function User Interface */
	// Open The Player Inventory UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | HUD")
	void OpenInventoryWindow();

	// Close The Player Inventory UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | HUD")
	void CloseInventoryWindow();

	// Open The Container Inventory UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | HUD")
	void OpenContainerWindow();

	// Close The Container Inventory UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | HUD")
	void CloseContainerWindow();

	// Open The Equipment Inventory UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | HUD")
	void OpenEquipmentWindow();

	// Close The Equipment Inventory UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | HUD")
	void CloseEquipmentWindow();

	// Creates The Players Inventory UI (Or Rebuilds If Already Loaded)
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	void  LoadInventory();

	// Creates The player Inventory UI Slots (Or Rebuilds If Already Loaded)
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	void CreateInventorySlots(int32 InvSize, uint8 SlotsPerRow);

	// Sets Inventory UI Slot Item Info
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	void SetInventorySlotItem(int32 InvSlot, FInventoryItem InvItem);

	// Clears Inventory UI Slot Item Info
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	void ClearInventorySlotItem(int32 InvSlot);

	// Gets Inventory UI Slot Item Info
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	FInventoryItem GetInventorySlotItem(int32 InvSlot);

	// Add Specified Amount Of Slots To The Players Inventory UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	void IncreaseInventorySlots(int32 Amount);

	// Removes UI Slots From End Of Inventory Slots
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	void DecreaseInventorySlots(int32 Amount);

	// Adds a new spesified slots to the inventory UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	void AddInventorySlot(int32 Row, int32 Column, int32 Slot);

	// Removes a spesified slots from the inventory UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	void RemoveInventorySlot(int32 Amount);

	// Clear Invetory UI and Referesh From Inventory
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	void RefreshInventorySlots();

	// Clears All Inventory UI Item info
	UFUNCTION(BlueprintCallable, Category = "User Interface | Inventory")
	void ClearInventorySlotItems();

	// Loads The Container Data Into The Container UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | Container")
	void LoadContainerSlots(FContainerInfo ContainerProeperties, FItemInformation ItemInfo);

	// Creates The Container Inventory UI Slots (Or Rebuilds If Already Loaded)
	UFUNCTION(BlueprintCallable, Category = "User Interface | Container")
	void CreateContainerSlots(int32 InvSize, uint8 SlotsPerRow);

	// Clears The Container Inventory UI Slot Item Info
	UFUNCTION(BlueprintCallable, Category = "User Interface | Container")
	void ClearContainerSlots();

	// Sets The Container Inventory UI Slot Item Info
	UFUNCTION(BlueprintCallable, Category = "User Interface | Container")
	void SetContainerSlotItem(int32 ContainerSlot, FItemInformation ItemInfo);

	// Clear container UI slot item info
	UFUNCTION(BlueprintCallable, Category = "User Interface | Container")
	void ClearContainerSlotItem(int32 ContainerSlot);

	// Set Container UI Slot Info for All viewing clients
	UFUNCTION(BlueprintCallable, Category = "User Interface | Container")
	void SetContainerSlotInfo(int32 ContainerSlot, FItemInformation ItemInfo);

	// Clear Container UI Slot Info for All viewing clients
	UFUNCTION(BlueprintCallable, Category = "User Interface | Container")
	void ClearContainerSlotInfo(int32 ContainerSlot);

	// Add a new spesified slot to the container UI
	UFUNCTION(BlueprintCallable, Category = "User Interface | Container")
	void AddContainerSlot(int32 Row, int32 Column, int32 Slot, bool bIsStorage);

	// Get Hotbar UI Slot Info
	UFUNCTION(BlueprintCallable, Category = "User Interface | Hotbar")
	FItemInformation GetHotbarSlotItem(int32 HotbarSlot);

	// Set Hotbar UI Slot Info
	UFUNCTION(BlueprintCallable, Category = "User Interface | Hotbar")
	void SetHotbarSlotItem(int32 HotbarSlot, FItemInformation ItemInfo);

	// Clear Hotbar UI Slot Info
	UFUNCTION(BlueprintCallable, Category = "User Interface | Hotbar")
	void ClearHotbarSlotItem(int32 HotbarSlot);

	// Add/Move/Swap Hotbar UI Slots Item
	UFUNCTION(BlueprintCallable, Category = "User Interface | Hotbar")
	void MoveHotbarSlotItem(int32 FromSlot, int32 ToHotBarSlot, bool bFromInventory, bool bFromHotbar);

	// When Using The Hotbar, Finds the First Item Of Same Type In Inventory And Tells The Server To 'Use' it
	UFUNCTION(BlueprintCallable, Category = "User Interface | Hotbar")
	void UseHotbarItem(int32 HotbarSlot);
	/* End Function user Interface */
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<FInventoryItem> Items;

	/* Components */
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInventoryComponent> PlayerInventory;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInventoryComponent> ContainerInventory;
	/* End Components */

	/* User Interface*/
	UPROPERTY(BlueprintReadOnly, Category = "Properties | User Interface")
	bool bIsInventoryOpen;

	UPROPERTY(BlueprintReadOnly, Category = "Properties | User Interface")
	bool bIsContainerOpen;

	UPROPERTY(BlueprintReadOnly, Category = "Properties | User Interface")
	bool bIsEquipmentOpen;

	UPROPERTY(BlueprintReadOnly, Category = "Properties | User Interface")
	uint8 InventorySlotsPerRow = 6;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Properties | User Interface")
	TObjectPtr<UInventoryLayoutWidget> InventoryUI;
	/* End User Interface*/

	/* Demo Player Stats */
	UPROPERTY(BlueprintReadOnly, Category = "Properties | Player Stats")
	int32 Damage;

	UPROPERTY(BlueprintReadOnly, Category = "Properties | Player Stats")
	int32 Armor;

	UPROPERTY(BlueprintReadOnly, Category = "Properties | Player Stats")
	int32 Strength;

	UPROPERTY(BlueprintReadOnly, Category = "Properties | Player Stats")
	int32 Dexterity;

	UPROPERTY(BlueprintReadOnly, Category = "Properties | Player Stats")
	int32 Intelligence;
	/* End Demo Player Stats */

	/* Another Properties */
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	int32 Gold;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	int32 InventorySize = 28;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<AActor> CurrentContainer;

public:	
	FORCEINLINE bool IsInventoryOpen() const { return bIsInventoryOpen; }
	FORCEINLINE bool IsContainerOpen() const { return bIsContainerOpen; }
	FORCEINLINE bool IsEquipmentOpen() const { return bIsEquipmentOpen; }
	FORCEINLINE UINT8 GetInventorySlotsPerRow() const { return InventorySlotsPerRow; }
	FORCEINLINE UInventoryComponent* GetPlayerInventory() const { return PlayerInventory; }
	FORCEINLINE UInventoryComponent* GetContainerInventory() const { return ContainerInventory; }
		
	/* Macros */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Macros")
	FORCEINLINE bool ItemIsValid(FInventoryItem InvItem) const { return InvItem.Icon != nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Macros")
	FORCEINLINE EItemType ItemType(FInventoryItem InvItem) const { return InvItem.ItemType; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Macros")
	FORCEINLINE bool ItemIsSame(FInventoryItem InvItem, FInventoryItem OtherItem) const { return InvItem.ID == OtherItem.ID && InvItem.Icon == OtherItem.Icon; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Macros")
	FORCEINLINE bool ItemIsStackable(FInventoryItem InvItem) const { return InvItem.bIsStackable; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Macros")
	FORCEINLINE int32 ItemAmount(FInventoryItem InvItem) const { return InvItem.Amount; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Macros")
	FORCEINLINE int32 ItemMaxStackSize(FInventoryItem InvItem) const { return InvItem.MaxStackSize; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Macros")
	FORCEINLINE int32 FreeStackSpace(FInventoryItem InvItem) const { return InvItem.MaxStackSize - InvItem.Amount; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Macros")
	FORCEINLINE bool ItemIsDroppable(FInventoryItem InvItem) const { return InvItem.bIsDroppable; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Macros")
	FORCEINLINE FItemInformation ItemToItemInfo(FInventoryItem InvItem) const {
		FItemInformation ItemInfo;
		ItemInfo.ID = InvItem.ID;
		ItemInfo.Icon = InvItem.Icon;
		ItemInfo.Name = InvItem.Name;
		ItemInfo.Type = InvItem.ItemType;
		ItemInfo.Quality = InvItem.Quality;
		ItemInfo.Amount = InvItem.Amount;
		return ItemInfo;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Macros")
	FORCEINLINE FWorldItem ItemToWorldItem(FInventoryItem InvItem) const {
		FWorldItem LocWorldItem;

		LocWorldItem.ID = InvItem.ID;
		LocWorldItem.Amount = InvItem.Amount;
		LocWorldItem.Name = InvItem.Name;
		return LocWorldItem;
	}
	/* End Macros */
};
