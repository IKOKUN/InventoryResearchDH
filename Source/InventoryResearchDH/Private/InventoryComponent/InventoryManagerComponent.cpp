// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent/InventoryManagerComponent.h"
#include "InventoryComponent/InventoryComponent.h"
#include "InventoryComponent/EquipmentInventoryComponent.h"
#include "Data/EquipmentSlots.h"
#include "Data/ItemInformation.h"
#include "UsableActor/ContainerActor.h"
#include "UsableActor/WorldActor.h"
#include "Controller/IRPlayerController.h"
#include "HUD/InventoryLayoutWidget.h"
#include "HUD/InventoryWidget.h"
#include "HUD/InventorySlotWidget.h"
#include "Components/UniformGridPanel.h"
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
	if (InventoryUI)
	{
		if (InventoryUI->InventoryWidgetComp)
		{
			if (InventoryUI->InventoryWidgetComp->InventoryGridPanel)
			{
				InventoryUI->InventoryWidgetComp->InventoryGridPanel->ClearChildren();
				CreateInventorySlots(InventorySize, InventorySlotsPerRow);
				RefreshInventorySlots();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Inventory Grid Panel Not Set"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Inventory Widget Comp Not Set"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
}

void UInventoryManagerComponent::CreateInventorySlots(int32 InvSize, uint8 SlotsPerRow)
{
	if (InvSize > 0)
	{
		int32 LocalSlotNumber = GetNumberOfEquipmentSlots();
		int32 LocalLoopCount = 0;

		// ceil for row up!
		// max for always make at least one row
		int32 LastIndex = UKismetMathLibrary::Max(UKismetMathLibrary::FCeil(InvSize / UKismetMathLibrary::Conv_ByteToFloat(SlotsPerRow)) - 1, 0);

		for (int32 i = 0; i <= LastIndex; i++) // Ubah < menjadi <= untuk mencakup LastIndex
		{
			for (int32 j = 0; j < UKismetMathLibrary::Conv_ByteToInt(SlotsPerRow); j++)
			{
				if (LocalLoopCount == InvSize) // Cek sebelum menambahkan slot
				{
					break; // Hentikan loop dalam jika LocalLoopCount mencapai InvSize
				}

				AddInventorySlot(i, j, LocalSlotNumber);
				LocalSlotNumber++;
				LocalLoopCount++;
			}
		}
	}
}

void UInventoryManagerComponent::SetInventorySlotItem(int32 InvSlot, FItemInformation InvItem)
{
	if (InventoryUI)
	{
		InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.ID = InvItem.ID;
		InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.Name = InvItem.Name;
		InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.Amount = InvItem.Amount;
		InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.Icon = InvItem.Icon;
		InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.Quality = InvItem.Quality;
		InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.Type = InvItem.Type;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
}

void UInventoryManagerComponent::ClearInventorySlotItem(int32 InvSlot)
{
	if (InventoryUI)
	{
		InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation = FItemInformation();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
}

FItemInformation UInventoryManagerComponent::GetInventorySlotItem(int32 InvSlot)
{
	if (InventoryUI)
	{
		return InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
		return FItemInformation();
	}
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
	ClearInventorySlotItems();
	int32 LocalIndex = 0;
	for (FInventoryItem InvItem : PlayerInventory->GetInventoryItems())
	{
		if (ItemIsValid(InvItem))
		{
			SetInventorySlotItem(LocalIndex, ItemToItemInfo(InvItem));
		}
		LocalIndex++;
	}
	UpdateEquippedStats();
}

void UInventoryManagerComponent::ClearInventorySlotItems()
{
	if (InventoryUI)
	{
		for (auto ItemInfo : InventoryUI->InventorySlotWidgets)
		{
			ItemInfo->InvSlotItemInformation = FItemInformation();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
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

bool UInventoryManagerComponent::TryToAddItemToInventory(UInventoryComponent* Inventory, FInventoryItem& Item)
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
		bool bSuccesAddToInv = TryToAddItemToInventory(PlayerInventory, LocInventoryItem);
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
	FInventoryItem LocalInvItem = PlayerInventory->GetInventoryItem(InvSlot);
	if (CurrentContainer)
	{
		// Is Storage Container Open
		if (CurrentContainer->GetCanStoreItems())
		{
			// Move Item To Container From Inventory
			switch (ItemType(LocalInvItem))
			{
			case EItemType::Miscellaneous:
			case EItemType::Consumable:
			case EItemType::Currency:
				if (TryToAddItemToInventory(ContainerInventory, LocalInvItem))
				{
					RemoveItem(PlayerInventory, InvSlot);
				}
				else
				{
					AddItem(PlayerInventory, InvSlot, LocalInvItem);
				}
				break;
			case EItemType::Equipment:
				if (InvSlot < GetNumberOfEquipmentSlots())
				{
					UseEquipmentItem(InvSlot, LocalInvItem, ContainerInventory);
				}
				else
				{
					if (TryToAddItemToInventory(ContainerInventory, LocalInvItem))
					{
						RemoveItem(PlayerInventory, InvSlot);
					}
					else
					{
						AddItem(PlayerInventory, InvSlot, LocalInvItem);
					}
				}
				break;
			default:
				break;
			}		
		}
		else
		{
			// use item from inventory
			switch (ItemType(LocalInvItem))
			{
			case EItemType::Equipment:
				UseEquipmentItem(InvSlot, LocalInvItem, ContainerInventory);
				break;
			case EItemType::Consumable:
				UseConsumableItem(InvSlot, LocalInvItem);
				break;
			default:
				break;
			}
		}
	}
}

void UInventoryManagerComponent::UseEquipmentItem(int32 InvSlot, FInventoryItem InvItem, UInventoryComponent* ToInventory)
{
	// Are We UnEquipping
	if (InvSlot < GetNumberOfEquipmentSlots())
	{
		// Check if the container/inventory has free space
		int32 LocEmptySlot = 0;
		if (ToInventory->GetEmptyInventorySpace(LocEmptySlot))
		{
			UnEquipItem(PlayerInventory, InvSlot, ToInventory, LocEmptySlot);
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
					5.0f,                 // Durasi pesan dalam detik
					FColor::Green,        // Warna pesan
					FString::Printf(TEXT("No Free Space")) // Pesan
				);

			}
		}
	}
	else
	{
		// Finds Empty Slot Of Type To Equip To
		int32 LocEmptySlot = 0;
		if (GetEmptyEquipmentSlotByType(ItemEquipSlot(InvItem), LocEmptySlot))
		{
			EquipFromInventory(InvSlot, LocEmptySlot);
		}
		else
		{
			LocEmptySlot = GetEquipmentSlotByType(ItemEquipSlot(InvItem));
			EquipFromInventory(InvSlot, LocEmptySlot);
		}
	}
}

void UInventoryManagerComponent::UseConsumableItem(int32 InvSlot, FInventoryItem InvItem)
{
	AIRPlayerController* LocPlayerController = Cast< AIRPlayerController>(GetOwner());
	if (LocPlayerController)
	{
		if (LocPlayerController->SetHealth(InvItem.Health, InvItem.Duration))
		{
			int32 LocAmount = 1;
			bool bWasFullAmountRemoved = RemoveFromItemAmount(InvItem, LocAmount);
			if (bWasFullAmountRemoved)
			{
				RemoveItem(PlayerInventory, InvSlot);
			}
			else
			{
				AddItem(PlayerInventory, InvSlot, InvItem);
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
					5.0f,                 // Durasi pesan dalam detik
					FColor::Green,        // Warna pesan
					FString::Printf(TEXT("Can't Use Item")) // Pesan
				);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller Not Set"));
	}
}

void UInventoryManagerComponent::EquipItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot)
{
	if (FromInv && ToInv)
	{
		if (FromInv != ToInv && FromInvSlot != ToInvSlot)
		{
			FInventoryItem LocInvItem = FromInv->GetInventoryItem(FromInvSlot);
			EEquipmentSlotsType LocEquipmentSlotType = ItemEquipSlot(LocInvItem);
			if (ItemType(LocInvItem) == EItemType::Equipment)
			{
				if (GetEquipmentTypeBySlot(ToInvSlot) == LocEquipmentSlotType)
				{
					FInventoryItem LocSwapInvItem = FromInv->GetInventoryItem(FromInvSlot);
					// Swap Item?
					if (ItemIsValid(LocSwapInvItem))
					{
						// Can The Swap Destination Inventory Store Item
						if (CanContainerStoreItem(FromInv))
						{
							// Swap Items
							AddItem(ToInv, ToInvSlot, LocInvItem);
							AddItem(FromInv, FromInvSlot, LocSwapInvItem);
						}
						else
						{
							if (GEngine)
							{
								GEngine->AddOnScreenDebugMessage(
									-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
									5.0f,                 // Durasi pesan dalam detik
									FColor::Green,        // Warna pesan
									FString::Printf(TEXT("CONTAINER CANNOT STORE ITEMS")) // Pesan
								);
							}
						}
					}
					else
					{
						// Move item
						AddItem(ToInv, ToInvSlot, LocInvItem);
						RemoveItem(FromInv, FromInvSlot);
					}
					UpdateEquippedStats();
				}
				else
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(
							-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
							5.0f,                 // Durasi pesan dalam detik
							FColor::Green,        // Warna pesan
							FString::Printf(TEXT("Item Can Not Be equippped In That Slot")) // Pesan
						);
					}
				}
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
						5.0f,                 // Durasi pesan dalam detik
						FColor::Green,        // Warna pesan
						FString::Printf(TEXT("Item Is Not Equippable")) // Pesan
					);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Not Set"));
	}
}

void UInventoryManagerComponent::UnEquipItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot)
{
	if (FromInv && ToInv)
	{
		if (FromInv != ToInv && FromInvSlot != ToInvSlot)
		{
			FInventoryItem LocInvItem = FromInv->GetInventoryItem(FromInvSlot);
			FInventoryItem LocSwapInvItem = FromInv->GetInventoryItem(FromInvSlot);
			EEquipmentSlotsType LocEquipmentSlotType = ItemEquipSlot(LocInvItem);

			if (ItemIsValid(LocSwapInvItem))
			{
				// Can The Swap Destination Inventory Store Item
				if (CanContainerStoreItem(FromInv))
				{
					if (ItemType(LocSwapInvItem) == EItemType::Equipment)
					{
						if (ItemEquipSlot(LocSwapInvItem) == LocEquipmentSlotType)
						{
							// Swap Items
							AddItem(ToInv, ToInvSlot, LocInvItem);
							AddItem(FromInv, FromInvSlot, LocSwapInvItem);
						}
						else
						{
							if (GEngine)
							{
								GEngine->AddOnScreenDebugMessage(
									-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
									5.0f,                 // Durasi pesan dalam detik
									FColor::Green,        // Warna pesan
									FString::Printf(TEXT("Item  Can't Be Equipped In That Slot")) // Pesan
								);
							}
						}
					}
					else
					{
						if (GEngine)
						{
							GEngine->AddOnScreenDebugMessage(
								-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
								5.0f,                 // Durasi pesan dalam detik
								FColor::Green,        // Warna pesan
								FString::Printf(TEXT("Item Is Not Equippable")) // Pesan
							);
						}
					}		
				}
				else
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(
							-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
							5.0f,                 // Durasi pesan dalam detik
							FColor::Green,        // Warna pesan
							FString::Printf(TEXT("CONTAINER CANNOT STORE ITEMS")) // Pesan
						);
					}
				}
			}
			else
			{
				UEquipmentInventoryComponent* LocEqInvComp = Cast<UEquipmentInventoryComponent>(ToInv);
				if (LocEqInvComp)
				{
					if (ToInvSlot < GetNumberOfEquipmentSlots())
					{
						if (GetEquipmentTypeBySlot(ToInvSlot) == LocEquipmentSlotType)
						{
							// Move Items
							AddItem(ToInv, ToInvSlot, LocInvItem);
							RemoveItem(FromInv, FromInvSlot);
						}
						else
						{
							if (GEngine)
							{
								GEngine->AddOnScreenDebugMessage(
									-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
									5.0f,                 // Durasi pesan dalam detik
									FColor::Green,        // Warna pesan
									FString::Printf(TEXT("Item Can Not Be Equipped In That Slot")) // Pesan
								);
							}
						}
					}
					else
					{
						// Move Items
						AddItem(ToInv, ToInvSlot, LocInvItem);
						RemoveItem(FromInv, FromInvSlot);
					}
				}
				else
				{
					// Move Items
					AddItem(ToInv, ToInvSlot, LocInvItem);
					RemoveItem(FromInv, FromInvSlot);

					UE_LOG(LogTemp, Error, TEXT("Inventory Component is not Equipment InventoryComponent"));
				}
			}
			UpdateEquippedStats();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Not Set"));
	}
}

void UInventoryManagerComponent::UpdateEquippedStats()
{
	int32 LocalDamage = 0;
	int32 LocalArmor = 0;
	int32 LocalStrength = 0;
	int32 LocalDexterity = 0;
	int32 LocalIntelligence = 0;

	for (int32 i = 0; i < GetNumberOfEquipmentSlots() - 1; i++)
	{
		FInventoryItem LocInvItem = PlayerInventory->GetInventoryItem(i);
		LocalDamage = LocalDamage + LocInvItem.Damage;
		LocalArmor = LocalArmor + LocInvItem.Armor;
		LocalStrength = LocalStrength + LocInvItem.Strength;
		LocalDexterity = LocalDexterity + LocInvItem.Dexterity;
		LocalIntelligence = LocalIntelligence + LocInvItem.Intelligence;
	}
	Damage = LocalDamage;
	Armor = LocalArmor;
	Strength = LocalStrength;
	Dexterity = LocalDexterity;
	Intelligence = LocalIntelligence;
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
	if (InvComp)
	{
		InvComp->SetInventoryItem(InvSlot, InvItem);
		FItemInformation ItemInfo = ItemToItemInfo(InvItem);
		if (PlayerInventory == InvComp)
		{
			// Update HUD Inventory Slot Info
			SetInventorySlotItem(InvSlot, ItemInfo);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Not Set"));
	}
}

void UInventoryManagerComponent::RemoveItem(UInventoryComponent* InvComp, int32 InvSlot)
{
	if (InvComp)
	{
		InvComp->ClearInventoryItem(InvSlot);
		if (PlayerInventory == InvComp)
		{
			// Update HUD Inventory Slot Info
			ClearInventorySlotItem(InvSlot);
		}
	}
}

void UInventoryManagerComponent::MoveItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot)
{
	if (FromInv && ToInv)
	{
		if (FromInv != ToInv && FromInvSlot != ToInvSlot)
		{
			if (CanContainerStoreItem(ToInv))
			{
				FInventoryItem LocalInvItem = FromInv->GetInventoryItem(FromInvSlot);
				if (ItemType(LocalInvItem) == EItemType::Currency)
				{
					AddGold(ItemAmount(LocalInvItem));
					RemoveItem(FromInv, FromInvSlot);
				}
				else
				{
					FInventoryItem LocalSwapInvItem = ToInv->GetInventoryItem(ToInvSlot);
					if (ItemIsValid(LocalSwapInvItem))
					{
						// Check if Items are Stackable
						if (ItemIsStackable(LocalSwapInvItem) && ItemFreeStackSpace(LocalSwapInvItem) > 0 && ItemIsSame(LocalInvItem, LocalSwapInvItem))
						{
							// Add To Stack
							int32 AmountRemaining = AddItemToStack(ToInv, ToInvSlot, ItemAmount(LocalInvItem));
							if (AmountRemaining > 0)
							{
								// Update The From Stack Amount
								SetItemAmount(LocalInvItem, AmountRemaining);
								AddItem(FromInv, FromInvSlot, LocalInvItem);
							}
							else
							{
								// Full Amoun Was Moved Clear Old Stack
								RemoveItem(FromInv, FromInvSlot);
							}

						}
						else
						{
							// Swap Item
							// We are trying to swap, but can the from inventory store items?
							if (CanContainerStoreItem(FromInv))
							{
								AddItem(ToInv, ToInvSlot, LocalInvItem);
								AddItem(FromInv, FromInvSlot, LocalSwapInvItem);
							}
							else
							{
								if (GEngine)
								{
									GEngine->AddOnScreenDebugMessage(
										-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
										5.0f,                 // Durasi pesan dalam detik
										FColor::Green,        // Warna pesan
										FString::Printf(TEXT("Container Can't Store Items")) // Pesan
									);

								}
							}
						}
					}
					else
					{
						// Move Item
						AddItem(ToInv, ToInvSlot, LocalInvItem);
						RemoveItem(FromInv, FromInvSlot);
					}
				}
			}
		}
	}
}

void UInventoryManagerComponent::DropItem(UInventoryComponent* InvComp, int32 InvSlot)
{
	if (InvComp)
	{
		FInventoryItem LocalInvItem = InvComp->GetInventoryItem(InvSlot);
		if (ItemIsDroppable(LocalInvItem))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
					5.0f,                 // Durasi pesan dalam detik
					FColor::Green,        // Warna pesan
					FString::Printf(TEXT("Dropped Item")) // Pesan
				);

			}
			FTransform RandomTransform = RandomizeDropLocation();
			// Define spawn parameters
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			// Spawn the actor
			AWorldActor* DroppedRandomActor = GetWorld()->SpawnActor<AWorldActor>(RandomDropClassActor, RandomTransform, SpawnParams);
			if (DroppedRandomActor)
			{
				// Generate random torque values
				int32 TorqueX = FMath::RandRange(-512, 512);
				int32 TorqueY = FMath::RandRange(-96, 96);
				int32 TorqueZ = FMath::RandRange(-32, 32);
				// Create a torque vector
				FVector Torque(TorqueX, TorqueY, TorqueZ);


				DroppedRandomActor->GetStaticMeshComponent()->AddTorqueInRadians(Torque, NAME_None, true);

				UE_LOG(LogTemp, Log, TEXT("Torque applied: %s"), *Torque.ToString());
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
						5.0f,                 // Durasi pesan dalam detik
						FColor::Green,        // Warna pesan
						FString::Printf(TEXT("Failed To Spawn Actor")) // Pesan
					);

				}
			}

		}
	}
}

bool UInventoryManagerComponent::HasItem(UInventoryComponent* InvComp, FName ItemID, int32& InvSlot)
{
	if (InvComp)
	{
		int32 LocInvSlot = 0;
		for (FInventoryItem LocInvItem : InvComp->GetInventoryItems())
		{
			if (LocInvItem.ID == ItemID)
			{
				InvSlot = LocInvSlot;
				return true;
			}
			LocInvSlot++;
		}
	}
	return false;
}

void UInventoryManagerComponent::AddGold(int32 Amount)
{
	Gold = Gold + Amount;
}

void UInventoryManagerComponent::SplitItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot, int32 Amount)
{
	if (FromInv && ToInv)
	{
		if (FromInv != ToInv && FromInvSlot != ToInvSlot)
		{
			// can the destination inventory store items?
			if (CanContainerStoreItem(ToInv))
			{
				FInventoryItem LocalFromInvItem = FromInv->GetInventoryItem(FromInvSlot);
				if (ItemType(LocalFromInvItem) == EItemType::Currency)
				{
					AddGold(ItemAmount(LocalFromInvItem));
					RemoveItem(FromInv, FromInvSlot);
				}
				else
				{
					FInventoryItem LocalSwapInvItem = ToInv->GetInventoryItem(ToInvSlot);
					// Are We Are Trying to Add The Split To An Item
					if (ItemIsValid(LocalSwapInvItem))
					{
						// Check if Items are Stackable
						// If Stack of same item and if not full
						if (ItemIsStackable(LocalSwapInvItem) && ItemFreeStackSpace(LocalSwapInvItem) > 0 && ItemIsSame(LocalFromInvItem, LocalSwapInvItem))
						{
							// Add To Stack
							int32 AmountRemaining = AddItemToStack(ToInv, ToInvSlot, Amount) - Amount;
							if (RemoveFromItemAmount(LocalFromInvItem, AmountRemaining))
							{
								// Remove From Stack
								RemoveItem(FromInv, FromInvSlot);
							}
							else
							{
								// Update From Stack
								AddItem(FromInv, FromInvSlot, LocalFromInvItem);
							}

						}
						else
						{
							if (GEngine)
							{
								GEngine->AddOnScreenDebugMessage(
									-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
									5.0f,                 // Durasi pesan dalam detik
									FColor::Green,        // Warna pesan
									FString::Printf(TEXT("Can't swap items while splitting stack")) // Pesan
								);

							}
						}
					}
					else
					{
						// Split Item To Empty Slot
						// Only Do This When We Are Successful In Our Split/Move
						bool bWasFullAmountRemoved = RemoveFromItemAmount(LocalFromInvItem, Amount);
						// Create The Split Item
						SetItemAmount(LocalFromInvItem, Amount);
						// Add The Split Item To Slot
						AddItem(ToInv, ToInvSlot, LocalFromInvItem);
						// Did We Split and Move The Full Stack?
						if (bWasFullAmountRemoved)
						{
							RemoveItem(FromInv, FromInvSlot);
						}
						else
						{
							AddItem(FromInv, FromInvSlot, LocalFromInvItem);
						}
					}
				}
			}
		}
	}
}

void UInventoryManagerComponent::IncreaseInventorySize(UInventoryComponent* InvComp, int32 Amount)
{
	if (InvComp)
	{
		// Validate Input
		if (Amount > 0)
		{
			InvComp->IncreaseInventorySize(Amount);
			// Are we updating the player inventory Size?
			if (PlayerInventory == InvComp)
			{
				IncreaseInventorySlots(Amount);
				InventorySize = InventorySize + Amount;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Not Set"));
	}
}

void UInventoryManagerComponent::DecreaseInventorySize(UInventoryComponent* InvComp, int32 Amount)
{
	if (InvComp)
	{
		// Validate Input
		if (Amount > 0)
		{
			InvComp->IncreaseInventorySize(Amount);
			// Are we updating the player inventory Size?
			if (PlayerInventory == InvComp)
			{
				UEquipmentInventoryComponent* LocEquipmentInventory = Cast<UEquipmentInventoryComponent>(InvComp);
				if (LocEquipmentInventory)
				{
					if (CheckSlotsForItems(InvComp, Amount))
					{
						if (GEngine)
						{
							GEngine->AddOnScreenDebugMessage(
								-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
								5.0f,                 // Durasi pesan dalam detik
								FColor::Green,        // Warna pesan
								FString::Printf(TEXT("You Must Remove Items From The Last %d Slots"), Amount) // Pesan
							);
						}
					}
					else
					{
						if (InvComp->GetInventoryItems().Num() - 1 >= GetNumberOfEquipmentSlots())
						{
							InvComp->DecreaseInventorySize(Amount);
							DecreaseInventorySlots(Amount);
							InventorySize = InventorySize - Amount;
						}
						else
						{
							if (GEngine)
							{
								GEngine->AddOnScreenDebugMessage(
									-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
									5.0f,                 // Durasi pesan dalam detik
									FColor::Green,        // Warna pesan
									FString::Printf(TEXT("TRYING TO REMOVE EQUIPMENT SLOTS")) // Pesan
								);
							}
						}
					}
					
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Casting To Equipment Inventory Component Failed"));
					InvComp->DecreaseInventorySize(Amount);
					InventorySize = InventorySize - Amount;
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Player Inventory And Inv Comp Not Same"));
				InvComp->DecreaseInventorySize(Amount);
				InventorySize = InventorySize - Amount;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Not Set"));
	}
}

bool UInventoryManagerComponent::CheckSlotsForItems(UInventoryComponent* InvComp, int32 Amount)
{
	for (int32 i = InvComp->GetInventoryItems().Num() - Amount; i > InvComp->GetInventoryItems().Num() - 1; i++)
	{
		if (ItemIsValid(InvComp->GetInventoryItem(i)))
		{
			return true;
		}
	}
	return false;
}
