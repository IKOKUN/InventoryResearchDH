// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent/InventoryManagerComponent.h"
#include "InventoryComponent/InventoryComponent.h"
#include "InventoryComponent/EquipmentInventoryComponent.h"
#include "Data/EquipmentSlots.h"
#include "Data/ItemInformation.h"
#include "UsableActor/ContainerActor.h"
#include "Controller/IRPlayerController.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
			5.0f,                 // Durasi pesan dalam detik
			FColor::Green,        // Warna pesan
			FString::Printf(TEXT("Equipment Slot Total : %d"), GetNumberOfEquipmentSlots()) // Pesan
		);

	}
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

void UInventoryManagerComponent::UnEquipFromInventory(int32 FromInvSlot, int32 ToInvSlot)
{
	UnEquipItem(PlayerInventory, FromInvSlot, PlayerInventory, ToInvSlot);
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
		UnEquipItem(PlayerInventory, FromInvSlot, ContainerInventory, ToContainerSlot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Container Inventory Not Set"));
	}
}

void UInventoryManagerComponent::SplitContainerItem(int32 FromContainerSlot, int32 ToContainerSlot, int32 Amount)
{
	SplitItem(ContainerInventory, FromContainerSlot, PlayerInventory, ToContainerSlot, Amount);
}

void UInventoryManagerComponent::SplitItemFromContainer(int32 FromContainerSlot, int32 ToSlot, int32 Amount)
{
	SplitItem(ContainerInventory, FromContainerSlot, PlayerInventory, ToSlot, Amount);
}

void UInventoryManagerComponent::LoadInventory()
{

}

void UInventoryManagerComponent::CreateInventorySlots(int32 InvSize, uint8 SlotsPerRow)
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

void UInventoryManagerComponent::LoadContainerSlots(FContainerInfo ContainerProeperties, TArray<FItemInformation> ItemInfo)
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
	FName LocItemID = Item.ID;
	int32 LocItemAmount = Item.Amount;
	if (Item.ItemType == EItemType::Currency)
	{
		AddGold(LocItemAmount);
		return true;
	}
	else if (Item.bIsStackable)
	{
		LocItemAmount = FindAndAddAmountToStacks(Inventory, LocItemID, LocItemAmount);
		if (LocItemAmount == 0)
		{
			return true;
		}
		else
		{
			SetItemAmount(Item, LocItemAmount);
		}
		if (Inventory)
		{
			int32 IndexEmptySlot = 0;
			if (Inventory->GetEmptyInventorySpace(IndexEmptySlot))
			{
				AddItem(Inventory, IndexEmptySlot, Item);
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Inventory Full"));
				return false;
			}
		}
	}
	return false;
}

bool UInventoryManagerComponent::TryRemoveItemFromInventory(UInventoryComponent* Inventory, FName ItemID, int32 Amount)
{
	int32 LocAmount = Amount;
	if (CheckInventoryForItemAmount(Inventory, ItemID, Amount))
	{
		int32 LocInventorySlot = 0;
		for (FInventoryItem LocInvItem : Inventory->GetInventoryItems())
		{
			if (LocInvItem.ID == ItemID)
			{
				FInventoryItem OutInvItem = LocInvItem;
				int32 OutAmount = 0;
				bool bWasFullAmountRemoved = RemoveFromItemAmount(OutInvItem, OutAmount);
				LocAmount = LocAmount - OutAmount;
				if (bWasFullAmountRemoved)
				{
					RemoveItem(Inventory, LocInventorySlot);
				}
				else
				{
					AddItem(Inventory, LocInventorySlot, OutInvItem);
				}

				if (LocAmount <= 0)
				{
					return true;
				}
			}
		
			LocInventorySlot++;

		}
	}

	return false;
}

bool UInventoryManagerComponent::CheckInventoryForItemAmount(UInventoryComponent* Inventory, FName ItemID, int32 Amount)
{
	int32 LocAmount = Amount;
	for (FInventoryItem LocInvItem : Inventory->GetInventoryItems())
	{
		if (LocInvItem.ID == ItemID)
		{
			Amount = Amount + LocInvItem.Amount;
			if (Amount >= LocAmount)
			{
				return true;
			}
		}
	}
	return false;
}

void UInventoryManagerComponent::UseContainer(AContainerActor* ContainerActor)
{
	// Memeriksa apakah objek mengimplementasikan IInventoryInterface
	if (ContainerActor && ContainerActor->Implements<UInventoryInterface>())
	{
		// Melakukan casting ke IInventoryInterface agar dapat memanggil fungsi-fungsi interface
		IInventoryInterface* ContainerInterface = Cast<IInventoryInterface>(ContainerActor);
		if (ContainerInterface)
		{
			// Panggil fungsi OnUse() dari interface
			if (CurrentContainer != ContainerActor)
			{
				OpenContainer(ContainerActor);
			}
			else
			{
				CloseContainer();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Container does not implement the IInventoryInterface."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Is Not A Valid Inventory Container Actor."));
	}
}

void UInventoryManagerComponent::OpenContainer(AContainerActor* ContainerActor)
{
	if (ContainerActor)
	{
		TArray<FItemInformation> LocItemInformation;
		CurrentContainer = ContainerActor;
		FContainerProperties LocContainerProperties = ContainerActor->GetContainerProperties();
		ContainerInventory = CurrentContainer->GetContainerInvetory();
		if (ContainerInventory)
		{
            for (FInventoryItem LocInvItem : ContainerInventory->GetInventoryItems())
			{
				LocItemInformation.Add(ItemToItemInfo(LocInvItem));
			}
			FContainerInfo ConInfo;
			ConInfo.Name = LocContainerProperties.ContainerName;
			ConInfo.StorageInventorySize = ContainerInventory->GetInventoryItems().Num();
			ConInfo.SlotsPerRow = LocContainerProperties.SlotsPerRow;
			ConInfo.bIsStorageContainer = LocContainerProperties.bIsStorageContainer;
			LoadContainerSlots(ConInfo, LocItemInformation);
		}
	}

}

void UInventoryManagerComponent::CloseContainer()
{
	CurrentContainer = nullptr;
	ContainerInventory = nullptr;

	CloseContainerWindow();
}

void UInventoryManagerComponent::UseContainerItem(int32 ContainerSlot)
{
	if (ContainerInventory)
	{
		FInventoryItem LocInventoryItem = ContainerInventory->GetInventoryItem(ContainerSlot);
		bool bSuccesAddToInv = TryAddItemToInventory(PlayerInventory, LocInventoryItem);
		if (bSuccesAddToInv)
		{
			RemoveItem(ContainerInventory, ContainerSlot);
		}
		else
		{
			AddItem(ContainerInventory, ContainerSlot, LocInventoryItem);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Container Inventory Not Set"));
	}
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

void UInventoryManagerComponent::UnEquipItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot)
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
	EEquipmentSlots LocEquipmentSlot = EEquipmentSlots::Head;
	if (EquipmentSlotIndex >= 0 && EquipmentSlotIndex < static_cast<int32>(EEquipmentSlots::OffHand) + 1)
	{
		LocEquipmentSlot = static_cast<EEquipmentSlots>(EquipmentSlotIndex);
		switch (LocEquipmentSlot)
		{
		case EEquipmentSlots::Head:
			return EEquipmentSlotsType::Head;
		case EEquipmentSlots::Shoulder:
			return EEquipmentSlotsType::Shoulder;
		case EEquipmentSlots::Chest:
			return EEquipmentSlotsType::Chest;
		case EEquipmentSlots::Hands:
			return EEquipmentSlotsType::Hands;
		case EEquipmentSlots::Legs:
			return EEquipmentSlotsType::Legs;
		case EEquipmentSlots::Feet:
			return EEquipmentSlotsType::Feet;
		case EEquipmentSlots::Accessory:
			return EEquipmentSlotsType::Accessory;
		case EEquipmentSlots::Back:
			return EEquipmentSlotsType::Back;
		case EEquipmentSlots::RightRing:
			return EEquipmentSlotsType::Ring;
		case EEquipmentSlots::LeftRing:
			return EEquipmentSlotsType::Ring;
		case EEquipmentSlots::Waist:
			return EEquipmentSlotsType::Waist;
		case EEquipmentSlots::Trinket:
			return EEquipmentSlotsType::Trinket;
		case EEquipmentSlots::MainHand:
			return EEquipmentSlotsType::MainHand;
		case EEquipmentSlots::OffHand:
			return EEquipmentSlotsType::OffHand;
		default:
			break;
		}
	}
	return EEquipmentSlotsType();
}

int32 UInventoryManagerComponent::GetEquipmentSlotByType(EEquipmentSlotsType EqSlotType)
{
	for (int32 i = 0; i < GetNumberOfEquipmentSlots() - 1; i++)
	{
		if (EqSlotType == GetEquipmentTypeBySlot(i))
		{
			return i;
		}
	}

	return int32();
}

bool UInventoryManagerComponent::GetEmptyEquipmentSlotByType(EEquipmentSlotsType EqSlotType, int32& OutEmptySlotIndex)
{
	for (int32 i = 0; i < GetNumberOfEquipmentSlots() - 1; i++)
	{
		if (EqSlotType == GetEquipmentTypeBySlot(i))
		{
			bool bInvItemValid = ItemIsValid(PlayerInventory->GetInventoryItem(i));

			if (!bInvItemValid && GetEquipmentTypeBySlot(i) == EqSlotType)
			{
				OutEmptySlotIndex = i;
				return true;
			}
		}
	}
	return false;
}

bool UInventoryManagerComponent::CanContainerStoreItem(UInventoryComponent* Inventory)
{
	if (CurrentContainer)
	{
		if (CurrentContainer->GetContainerInvetory() == Inventory)
		{
			if (CurrentContainer->GetCanStoreItems())
			{
				return true;
			}
			return false;
		}
		return true;
	}
	return true;
}

FTransform UInventoryManagerComponent::RandomizeDropLocation()
{
	FVector LocalLocation = FVector(0.f, 0.f, -16.f);
	FTransform LocTransform;
	AIRPlayerController* LocPlayerController = Cast< AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (LocPlayerController)
	{
		FVector PawnLocation = LocPlayerController->GetPawn()->GetActorLocation();
		// Drop Distance Range From Player
		FVector RandomLocation = FVector(FMath::RandRange(64, 96), 0.f, 0.f);
		// Drop Item 360 degrees around player
		FRotator RandomRotation = FRotator(0.f, FMath::RandRange(-180, 180), 0.f);

        FVector RotateRandomVector = FRotationMatrix(RandomRotation).TransformVector(RandomLocation);

		LocalLocation = LocalLocation + PawnLocation + RotateRandomVector;

		// Give The Droppped Obect a Random Rotation
		FRotator DroppedObjectRandomRotation = FRotator(0.f, FMath::RandRange(-18, 18) * 10, 0.f);
		LocTransform = FTransform(DroppedObjectRandomRotation, LocalLocation, FVector(1.f, 1.f, 1.f));
	}

	return LocTransform;
}

int32 UInventoryManagerComponent::FindAndAddAmountToStacks(UInventoryComponent* Inventory, FName ItemID, int32 Amount)
{
	int32 LocInvSlot = 0;
	for (FInventoryItem InvItem : Inventory->GetInventoryItems())
	{
		if (InvItem.ID == ItemID && InvItem.bIsStackable)
		{
			Amount = AddItemToStack(Inventory, LocInvSlot, Amount);
			if (Amount == 0)
			{
				return Amount;
			}
		}

		LocInvSlot++;
	}
	return Amount;
}

int32 UInventoryManagerComponent::AddItemToStack(UInventoryComponent* Inventory, int32 InvSlot, int32 AmountToAdd)
{
	if (Inventory)
	{
		FInventoryItem LocInvItem = Inventory->GetInventoryItem(InvSlot);
		int32 LocItemAmount = ItemAmount(LocInvItem);
		int32 LocMaxStackSize = ItemMaxStackSize(LocInvItem);
		int32 LocRemainingAmount = AmountToAdd;
		if (LocItemAmount < LocMaxStackSize)
		{
			int32 FreeStackSpace = LocMaxStackSize - LocItemAmount;
			if (AmountToAdd <= FreeStackSpace)
			{
				LocRemainingAmount = 0;
				AddItem(Inventory, InvSlot, AddToItemAmount(LocInvItem, AmountToAdd));
				return LocRemainingAmount;
			}
			else
			{
				LocRemainingAmount = AmountToAdd - FreeStackSpace;
				AddItem(Inventory, InvSlot, AddToItemAmount(LocInvItem, FreeStackSpace));
				return LocRemainingAmount;
			}
			
		}
		else
		{
			return LocRemainingAmount;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Not Set"));
		return int32();
	}
}

FInventoryItem UInventoryManagerComponent::SetItemAmount(FInventoryItem Inventory, int32 AmountToAdd)
{
	FInventoryItem LocInvItem = Inventory;
	LocInvItem.Amount = AmountToAdd;
	return LocInvItem;
}

FInventoryItem UInventoryManagerComponent::AddToItemAmount(FInventoryItem InvItem, int32 AmountToAdd)
{
	FInventoryItem LocInvItem = InvItem;
	LocInvItem.Amount = LocInvItem.Amount + AmountToAdd;
	return LocInvItem;
}

bool UInventoryManagerComponent::RemoveFromItemAmount(FInventoryItem& OutInvItem, int32& AmountToRemove)
{
	int32 LocAmountRemoved = 0;
	if (AmountToRemove >= ItemAmount(OutInvItem))
	{
		LocAmountRemoved = ItemAmount(OutInvItem);
		AmountToRemove = LocAmountRemoved;
		return true;
	}
	else
	{
		LocAmountRemoved = AmountToRemove;
		SetItemAmount(OutInvItem, ItemAmount(OutInvItem) - AmountToRemove);
		return false;
	}
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
