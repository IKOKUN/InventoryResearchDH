// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent/InventoryManagerComponent.h"

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UInventoryManagerComponent::InitInventoryItems()
{
	if (PlayerInventory)
	{
		PlayerInventory->InitInventory(InventorySize);
	}
	else
	{
        UE_LOG(LogTemp, Error, TEXT("Player Inventory Not Set"));
	}
}

void UInventoryManagerComponent::MoveInventoryItem(int32 FromInvSlot, int32 ToInvSlot)
{
	MoveItem(PlayerInventory, FromInvSlot, PlayerInventory, ToInvSlot);
}

void UInventoryManagerComponent::EquipFromInventory(int32 FromInvSlot, int32 ToInvSlot)
{
	EquipItem(PlayerInventory, FromInvSlot, PlayerInventory, ToInvSlot);
}

void UInventoryManagerComponent::UnequipFromInventory(int32 FromInvSlot, int32 ToInvSlot)
{
	UnequipItem(PlayerInventory, FromInvSlot, PlayerInventory, ToInvSlot);
}

void UInventoryManagerComponent::DropItemFromInventory(int32 FromInvSlot)
{
	if (PlayerInventory)
	{
		DropItem(PlayerInventory, FromInvSlot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player Inventory Not Set"));
	}
}

void UInventoryManagerComponent::SplitInventoryItem(int32 FromInvSlot, int32 ToInvSlot, int32 Amount)
{
	SplitItem(PlayerInventory, FromInvSlot, PlayerInventory, ToInvSlot, Amount);
}

void UInventoryManagerComponent::SplitItemFromInventory(int32 FromInvSlot, int32 ToInvSlot, int32 Amount)
{
	SplitItem(PlayerInventory, FromInvSlot, ContainerInventory, ToInvSlot, Amount);
}

void UInventoryManagerComponent::EventIncreaseInventorySize(int32 Amount)
{
	IncreaseInventorySize(PlayerInventory, Amount);
}

void UInventoryManagerComponent::EventDecreaseInventorySize(int32 Amount)
{
	DecreaseInventorySize(PlayerInventory, Amount);
}

void UInventoryManagerComponent::MoveContainerItem(int32 FromContainerSlot, int32 ToContainerSlot)
{
	MoveItem(ContainerInventory, FromContainerSlot, ContainerInventory, ToContainerSlot);
}

void UInventoryManagerComponent::TakeContainerItem(int32 FromContainerSlot, int32 ToContainerSlot)
{
	if (ContainerInventory)
	{
		MoveItem(ContainerInventory, FromContainerSlot, PlayerInventory, ToContainerSlot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Container Inventory Not Set"));
	}
}

void UInventoryManagerComponent::DepositContainerItem(int32 FromInvSlot, int32 ToContainerSlot)
{
	if (ContainerInventory)
	{
		MoveItem(PlayerInventory, FromInvSlot, ContainerInventory, ToContainerSlot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Container Inventory Not Set"));
	}
}

void UInventoryManagerComponent::EquipFromContainer(int32 FromContainerSlot, int32 ToInvSlot)
{
	if (ContainerInventory)
	{
		EquipItem(ContainerInventory, FromContainerSlot, PlayerInventory, ToInvSlot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Container Inventory Not Set"));
	}
}

void UInventoryManagerComponent::UnEquipToContainer(int32 FromInvSlot, int32 ToContainerSlot)
{
	if (ContainerInventory)
	{
		UnequipItem(PlayerInventory, FromInvSlot, ContainerInventory, ToContainerSlot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Container Inventory Not Set"));
	}
}

void UInventoryManagerComponent::SplitContainerItem(int32 FromContainerSlot, int32 ToContainerSlot)
{
	SplitItem(ContainerInventory, FromContainerSlot, ContainerInventory, ToContainerSlot, 1);

}

void UInventoryManagerComponent::SplitItemFromContainer(int32 FromContainerSlot, int32 ToSlot, int32 Amount)
{
	SplitItem(ContainerInventory, FromContainerSlot, PlayerInventory, ToSlot, Amount);
}

void UInventoryManagerComponent::LoadInventory()
{

}

void UInventoryManagerComponent::CreateInventorySlots(int32 InvSize, UINT8 SlotsPerRow)
{
}

void UInventoryManagerComponent::SetInventorySlotItem(int32 InvSlot, FInventoryItem InvItem)
{
}

void UInventoryManagerComponent::ClearInventorySlotItem(int32 InvSlot)
{
}

FInventoryItem UInventoryManagerComponent::GetInventorySlotItem(int32 InvSlot)
{
	return FInventoryItem();
}

void UInventoryManagerComponent::IncreaseInventorySlots(int32 Amount)
{
}

void UInventoryManagerComponent::DecreaseInventorySlots(int32 Amount)
{
}

void UInventoryManagerComponent::AddInventorySlot(int32 Row, int32 Column, int32 Slot)
{
}

void UInventoryManagerComponent::RemoveInventorySlot(int32 Amount)
{
}

void UInventoryManagerComponent::RefreshInventorySlots()
{
}

void UInventoryManagerComponent::ClearInventorySlotItems()
{
}

void UInventoryManagerComponent::LoadContainerSlots(FContainerInfo ContainerProeperties, FItemInformation ItemInfo)
{
}

void UInventoryManagerComponent::CreateContainerSlots(int32 InvSize, uint8 SlotsPerRow)
{
}

void UInventoryManagerComponent::ClearContainerSlots()
{
}

void UInventoryManagerComponent::SetContainerSlotItem(int32 ContainerSlot, FItemInformation ItemInfo)
{
}

void UInventoryManagerComponent::ClearContainerSlotItem(int32 ContainerSlot)
{
}

void UInventoryManagerComponent::SetContainerSlotInfo(int32 ContainerSlot, FItemInformation ItemInfo)
{
}

void UInventoryManagerComponent::ClearContainerSlotInfo(int32 ContainerSlot)
{
}

void UInventoryManagerComponent::AddContainerSlot(int32 Row, int32 Column, int32 Slot, bool bIsStorage)
{
}

FItemInformation UInventoryManagerComponent::GetHotbarSlotItem(int32 HotbarSlot)
{
	return FItemInformation();
}

void UInventoryManagerComponent::SetHotbarSlotItem(int32 HotbarSlot, FItemInformation ItemInfo)
{
}

void UInventoryManagerComponent::ClearHotbarSlotItem(int32 HotbarSlot)
{
}

void UInventoryManagerComponent::MoveHotbarSlotItem(int32 FromSlot, int32 ToHotBarSlot, bool bFromInventory, bool bFromHotbar)
{
}

void UInventoryManagerComponent::UseHotbarItem(int32 HotbarSlot)
{
}

void UInventoryManagerComponent::InitInventoryManagerUI(UInventoryLayoutWidget* InventoryWidget)
{
	InventoryUI = InventoryWidget;
}

void UInventoryManagerComponent::InitInventoryManager(UInventoryComponent* PlayerInventoryRef)
{
	PlayerInventory = PlayerInventoryRef;
}

bool UInventoryManagerComponent::TryAddItemToInventory(UInventoryComponent* Inventory, FInventoryItem& Item)
{
	return false;
}

bool UInventoryManagerComponent::TryRemoveItemFromInventory(UInventoryComponent* Inventory, FName ItemID, int32 Amount)
{
	return false;
}

bool UInventoryManagerComponent::CheckInventoryForItemAmount(UInventoryComponent* Inventory, FName ItemID, int32 Amount)
{
	return false;
}

void UInventoryManagerComponent::UseContainer(AActor* ContainerActor)
{

}

void UInventoryManagerComponent::OpenContainer(AActor* ContainerActor)
{
}

void UInventoryManagerComponent::CloseContainer()
{
}

void UInventoryManagerComponent::UseContainerItem(int32 ContainerSlot)
{
}

void UInventoryManagerComponent::UseInventoryItem(int32 InvSlot)
{
}

void UInventoryManagerComponent::UseEquipmentItem(int32 InvSlot, FInventoryItem InvItem, UInventoryComponent* ToInventory)
{
}

void UInventoryManagerComponent::UseConsumableItem(int32 InvSlot, FInventoryItem InvItem)
{
}

void UInventoryManagerComponent::EquipItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot)
{
}

void UInventoryManagerComponent::UnequipItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot)
{
}

void UInventoryManagerComponent::UpdateEquippedStats()
{
}

void UInventoryManagerComponent::OpenInventoryWindow()
{
}

void UInventoryManagerComponent::CloseInventoryWindow()
{
}

void UInventoryManagerComponent::OpenContainerWindow()
{
}

void UInventoryManagerComponent::CloseContainerWindow()
{
}

void UInventoryManagerComponent::OpenEquipmentWindow()
{
}

void UInventoryManagerComponent::CloseEquipmentWindow()
{
}

EEquipmentSlotsType UInventoryManagerComponent::GetEquipmentTypeBySlot(int32 EquipmentSlotIndex)
{
	return EEquipmentSlotsType();
}

int32 UInventoryManagerComponent::GetEquipmentSlotByType(EEquipmentSlotsType EqSlotType)
{
	return int32();
}

bool UInventoryManagerComponent::GetEmptyEquipmentSlotByType(EEquipmentSlotsType EqSlotType, int32& OutEmptySlotIndex)
{
	return false;
}

bool UInventoryManagerComponent::CanContainerStoreItem(UInventoryComponent* Inventory)
{
	return false;
}

FTransform UInventoryManagerComponent::RandomizeDropLocation()
{
	return FTransform();
}

int32 UInventoryManagerComponent::FindAndAddAmountToStacks(UInventoryComponent* Inventory, FName ItemID, int32 Amount)
{
	return int32();
}

int32 UInventoryManagerComponent::AddItemToStack(UInventoryComponent* Inventory, int32 InvSlot, int32 AmountToAdd)
{
	return int32();
}

int32 UInventoryManagerComponent::SetItemAmount(UInventoryComponent* Inventory, int32 InvSlot, int32 AmountToAdd)
{
	return int32();
}

int32 UInventoryManagerComponent::AddToItemAmount(FInventoryItem InvItem, int32 AmountToAdd)
{
	return int32();
}

bool UInventoryManagerComponent::RemoveFromItemAmount(FInventoryItem InvItem, int32& AmountToRemove)
{
	return false;
}

void UInventoryManagerComponent::AddItem(UInventoryComponent* InvComp, int32 InvSlot, FInventoryItem InvItem)
{
}

void UInventoryManagerComponent::RemoveItem(UInventoryComponent* InvComp, int32 InvSlot)
{
}

void UInventoryManagerComponent::MoveItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot)
{
}

void UInventoryManagerComponent::DropItem(UInventoryComponent* InvComp, int32 InvSlot)
{
}

bool UInventoryManagerComponent::HasItem(UInventoryComponent* InvComp, FName ItemID, int32& InvSlot)
{
	return false;
}

void UInventoryManagerComponent::AddGold(int32 Amount)
{
}

void UInventoryManagerComponent::SplitItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot, int32 Amount)
{
}

void UInventoryManagerComponent::IncreaseInventorySize(UInventoryComponent* InvComp, int32 Amount)
{
}

void UInventoryManagerComponent::DecreaseInventorySize(UInventoryComponent* InvComp, int32 Amount)
{
}

bool UInventoryManagerComponent::CheckSlotsForItems(UInventoryComponent* InvComp, int32 Amount)
{
	return false;
}
