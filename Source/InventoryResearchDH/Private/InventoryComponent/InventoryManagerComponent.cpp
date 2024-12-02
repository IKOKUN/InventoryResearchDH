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
#include "HUD/ContainerSlotWidget.h"
#include "HUD/ContainerWidget.h"
#include "HUD/HotbarWidget.h"
#include "HUD/HotbarSlotWidget.h"
#include "HUD/EquipmentWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Widget.h"
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
	FInventoryItem LocInvItem = PlayerInventory->GetInventoryItem(FromInvSlot);
	if (ItemIsValid(LocInvItem))
	{
		EquipItem(PlayerInventory, FromInvSlot, PlayerInventory, ToInvSlot);
		//if (ToInvSlot < GetNumberOfEquipmentSlots())
		//{
		//	EquipItem(PlayerInventory, FromInvSlot, PlayerInventory, ToInvSlot);
		//}
		//else
		//{
		//	// Equip Item
		//	EquipItem(PlayerInventory, FromInvSlot, PlayerInventory, ToInvSlot);
		//}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Item Not Valid"));
	}
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
		int32 LastIndex = UKismetMathLibrary::Max(UKismetMathLibrary::FCeil(InvSize / UKismetMathLibrary::Conv_ByteToDouble(SlotsPerRow)) - 1, 0);

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
		if (InventoryUI->InventorySlotWidgets[InvSlot])
		{
			InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.ID = InvItem.ID;
			InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.Name = InvItem.Name;
			InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.Amount = InvItem.Amount;
			InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.Icon = InvItem.Icon;
			InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.Quality = InvItem.Quality;
			InventoryUI->InventorySlotWidgets[InvSlot]->InvSlotItemInformation.Type = InvItem.Type;

			if (InventoryUI->InventorySlotWidgets[InvSlot]->InventorySlotIndex == 0)
			{
				InventoryUI->InventorySlotWidgets[InvSlot]->InventorySlotIndex = InvSlot;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Inventory Slot Widget Not Set"));
		}
		
		//UE_LOG(LogTemp, Error, TEXT("Inv Slot On InvSLotItem : %d"), InventoryUI->InventorySlotWidgets[InvSlot]->InventorySlotIndex);
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
		UE_LOG(LogTemp, Error, TEXT("InventorySlotWidgets index %d: "), InvSlot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
		return FItemInformation();
	}
}

void UInventoryManagerComponent::IncreaseInventorySlots(int32 Amount)
{
	if (Amount > 0 && InventoryUI)
	{
		int32 LocalSlotNumber = InventoryUI->InventorySlotWidgets.Num();
		int32 LocalNewInvSize = Amount + LocalSlotNumber;
		int32 LocalInvChildCountIndex = InventoryUI->InventoryWidgetComp->InventoryGridPanel->GetChildrenCount() - 1;
		int32 LocalRow = 0;
		int32 LocalColumn = 0;
		int32 LocalLoopCount = 0;
		// Mendapatkan referensi ke Inventory Grid Panel
		UUniformGridPanel* InventoryGridPanel = InventoryUI->InventoryWidgetComp->InventoryGridPanel;
		if (InventoryGridPanel->GetChildAt(LocalInvChildCountIndex))
		{
			// Casting ke UUniformGridSlot untuk mendapatkan Row dan Column
			UUniformGridSlot* LastSlot = Cast<UUniformGridSlot>(InventoryGridPanel->GetChildAt(LocalInvChildCountIndex)->Slot);
			if (LastSlot)
			{
				LocalRow = LastSlot->GetRow();
				LocalColumn = LastSlot->GetColumn();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Last Slot Not Set"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Last Slot Not Found"));
		}

		// ceil for row up!
		// max for always make at least one row
		int32 LastIndex = UKismetMathLibrary::Max(UKismetMathLibrary::FCeil(LocalNewInvSize / UKismetMathLibrary::Conv_ByteToDouble(InventorySlotsPerRow)) - 1, 0);

		int32 RowFirstIndex = LocalRow;
		int32 RowLastIndex = LastIndex;
		// Tukar Jika ternyata First Index Lebih Besar dari last index
		if (RowFirstIndex > RowLastIndex)
		{
			std::swap(RowFirstIndex, RowLastIndex);
		}

		int32 ColoumFirstIndex = LocalColumn;
		int32 ColoumLastIndex = UKismetMathLibrary::Conv_ByteToInt(InventorySlotsPerRow) - 1;
		// Tukar Jika ternyata First Index Lebih Besar dari last index
		if (RowFirstIndex > RowLastIndex)
		{
			std::swap(RowFirstIndex, RowLastIndex);
		}
		if (ColoumFirstIndex > ColoumLastIndex)
		{
			std::swap(ColoumFirstIndex, ColoumLastIndex);
		}
		// Tambahkan logika untuk menambah slot baru di sini
		for (int32 i = RowFirstIndex; i < RowLastIndex; i++)
		{
			for (int32 j = ColoumFirstIndex; j < ColoumLastIndex; j++)
			{
				if (LocalLoopCount != Amount)
				{
					AddInventorySlot(i, j, LocalSlotNumber);
					LocalSlotNumber++;
					LocalLoopCount++;
				}
				else
				{
					break;
				}
			}
			LocalColumn = 0;
			if (LocalLoopCount == Amount)
			{
				return;
			}
		}
	}
}

void UInventoryManagerComponent::DecreaseInventorySlots(int32 Amount)
{
	// Only create new UI slots if an actual amount is passed in
	if (Amount > 0 && InventoryUI)
	{
		int32 LastIndex = InventoryUI->InventoryWidgetComp->InventorySlotWidgets.Num();
		TArray<int32> LocalAmountIndexes;

		// Only Create New UI Slots If An Actual Amount Is Passed In
		for (int32 i = 0; i < LastIndex - Amount; i++)
		{
			LocalAmountIndexes.Add(i);
		}

		// Reverse The Loop
		for (int32 j = 0; j < LocalAmountIndexes.Last(); j++)
		{
			RemoveInventorySlot(LocalAmountIndexes[LocalAmountIndexes.Last() - j]);
		}
	}
}

void UInventoryManagerComponent::AddInventorySlot(int32 Row, int32 Column, int32 Slot)
{
	if (!InventoryUI)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Invalid"));
		return;
	}

	int32 LastIndex = InventoryUI->InventoryWidgetComp->InventorySlotWidgets.Num();
	UInventorySlotWidget* LocalSlotRef = nullptr; // Inisialisasi dengan nullptr

	// Pastikan Slot tidak melebihi LastIndex
	if (Slot >= 0 && Slot < LastIndex)
	{
		UInventorySlotWidget* NewSlot = InventoryUI->InventoryWidgetComp->InventorySlotWidgets[Slot];
		if (NewSlot)
		{
			NewSlot->InvSlotItemInformation = FItemInformation();
			LocalSlotRef = NewSlot;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("New Slot Not Set"));
		}
	}
	else
	{
		// Jika Slot tidak ada, buat slot baru
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(GetWorld(), InventorySlotWidgetClass);
		if (NewSlot)
		{
			LocalSlotRef = NewSlot;
			LocalSlotRef->InventorySlotIndex = Slot;
			InventoryUI->InventorySlotWidgets.Add(LocalSlotRef);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("New Slot Not Set"));
			return;
		}
	}

	// Pastikan LocalSlotRef diinisialisasi sebelum digunakan
	if (LocalSlotRef)
	{
		UPanelSlot* LocalSlotChild = InventoryUI->InventoryWidgetComp->InventoryGridPanel->AddChild(LocalSlotRef);
		UUniformGridSlot* NewSlotSlot = Cast<UUniformGridSlot>(LocalSlotChild);
		if (NewSlotSlot)
		{
			NewSlotSlot->SetRow(Row);
			NewSlotSlot->SetColumn(Column);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("New Slot Slot Casting Failed On Inventory Manager Component"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LocalSlotRef is not initialized."));
	}
}

void UInventoryManagerComponent::RemoveInventorySlot(int32 InvSlotIndex)
{
	if (InventoryUI)
	{
		if (InventoryUI->InventorySlotWidgets.Num() > InvSlotIndex)
		{
			InventoryUI->InventoryWidgetComp->InventoryGridPanel->RemoveChildAt(InvSlotIndex - GetNumberOfEquipmentSlots());
			InventoryUI->InventorySlotWidgets.RemoveAt(InvSlotIndex);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Inventory Slot Index Out Of Range"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
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
			UE_LOG(LogTemp, Error, TEXT("Refresh Inventory Slot"));
		}
		else
		{
			// UE_LOG(LogTemp, Error, TEXT("Invalid Inventory Item"));
		}
		LocalIndex++;
	}

	for (UInventorySlotWidget* SlotWidget : InventoryUI->InventorySlotWidgets)
	{
		UE_LOG(LogTemp, Error, TEXT("Load Inventory Name : %s"), *SlotWidget->InvSlotItemInformation.ID.ToString());
		UE_LOG(LogTemp, Error, TEXT("Load Inventory Slot : %d"), SlotWidget->InventorySlotIndex);
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
	if (InventoryUI)
	{
		if (InventoryUI->ContainerWidgetComp)
		{
			InventoryUI->ContainerWidgetComp->ContainerName = ContainerProeperties.Name;
			InventoryUI->ContainerWidgetComp->bIsStorageContainer = ContainerProeperties.bIsStorageContainer;
			if (InventoryUI->ContainerWidgetComp->bIsStorageContainer)
			{
				CreateContainerSlots(ContainerProeperties.StorageInventorySize, ContainerProeperties.SlotsPerRow);
			}
			else
			{
				CreateContainerSlots(ItemInfo.Num(), ContainerProeperties.SlotsPerRow);
			}
			for (int32  i = 0; i < ItemInfo.Num(); i++)
			{
				SetContainerSlotItem(i, ItemInfo[i]);
			}
			OpenContainerWindow();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Container Widget Comp Not Set"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
}

void UInventoryManagerComponent::CreateContainerSlots(int32 ContainerSize, uint8 SlotsPerRow)
{
	ClearContainerSlots();
	if (ContainerSize > 0)
	{
		if (InventoryUI)
		{
			if (InventoryUI->ContainerWidgetComp)
			{
				bool bLocalIsStorage = InventoryUI->ContainerWidgetComp->bIsStorageContainer;
				uint8 LocalMaxRowSize = SlotsPerRow;
				int32 LocalLoopCount = 0;
				// Is A Float To Handle Creating Partial Rows
				// Round Rows Up!
				// Always Make Atleast One Row
				int32 LastIndex = UKismetMathLibrary::Max(UKismetMathLibrary::FCeil(ContainerSize / UKismetMathLibrary::Conv_ByteToDouble(LocalMaxRowSize)) - 1, 0);
				for (int32 i = 0; i < LastIndex; i++)
				{
					for (int32 j = 0; j < UKismetMathLibrary::Conv_ByteToInt(LocalMaxRowSize) - 1; j++)
					{
						AddContainerSlot(i, j, LocalLoopCount, bLocalIsStorage);
						LocalLoopCount++;
						if (LocalLoopCount == ContainerSize)
						{
							break;
						}
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Container Widget Comp Not Set"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
		}
	}
}

void UInventoryManagerComponent::ClearContainerSlots()
{
	if (InventoryUI)
	{
		if (InventoryUI->ContainerWidgetComp)
		{
			InventoryUI->ContainerWidgetComp->ContainerSlots.Empty();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Container Widget Comp Not Set"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
}

void UInventoryManagerComponent::SetContainerSlotItem(int32 ContainerSlot, FItemInformation ItemInfo)
{
	if (InventoryUI)
	{
		if (InventoryUI->ContainerWidgetComp)
		{
			InventoryUI->ContainerWidgetComp->ContainerSlots[ContainerSlot]->ItemInformation.ID = ItemInfo.ID;
			InventoryUI->ContainerWidgetComp->ContainerSlots[ContainerSlot]->ItemInformation.Name = ItemInfo.Name;
			InventoryUI->ContainerWidgetComp->ContainerSlots[ContainerSlot]->ItemInformation.Amount = ItemInfo.Amount;
			InventoryUI->ContainerWidgetComp->ContainerSlots[ContainerSlot]->ItemInformation.Icon = ItemInfo.Icon;
			InventoryUI->ContainerWidgetComp->ContainerSlots[ContainerSlot]->ItemInformation.Quality = ItemInfo.Quality;
			InventoryUI->ContainerWidgetComp->ContainerSlots[ContainerSlot]->ItemInformation.Type = ItemInfo.Type;

			if (!InventoryUI->ContainerWidgetComp->bIsStorageContainer)
			{
				if (!InventoryUI->ContainerWidgetComp->ContainerSlots[ContainerSlot]->ItemInformation.Icon)
				{
					InventoryUI->ContainerWidgetComp->ContainerGridPanel->GetChildAt(ContainerSlot)->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Container Widget Comp Not Set"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
}

void UInventoryManagerComponent::ClearContainerSlotItem(int32 ContainerSlot)
{
	if (InventoryUI)
	{
		if (InventoryUI->ContainerWidgetComp)
		{
			InventoryUI->ContainerWidgetComp->ContainerSlots[ContainerSlot]->ItemInformation = FItemInformation();

			if (!InventoryUI->ContainerWidgetComp->bIsStorageContainer)
			{
				InventoryUI->ContainerWidgetComp->ContainerGridPanel->GetChildAt(ContainerSlot)->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Container Widget Comp Not Set"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
}

void UInventoryManagerComponent::AddContainerSlot(int32 Row, int32 Column, int32 Slot, bool bIsStorage)
{
	if (InventoryUI)
	{
		if (InventoryUI->ContainerWidgetComp)
		{
			UContainerSlotWidget* NewSlot = CreateWidget<UContainerSlotWidget>(GetWorld(), ContainerSlotWidgetClass);
			if (NewSlot)
			{
				UPanelSlot* LocalSlotChild = InventoryUI->ContainerWidgetComp->ContainerGridPanel->AddChild(NewSlot);
				UGridSlot* NewSlotSlot = Cast<UGridSlot>(LocalSlotChild);
				if (NewSlotSlot)
				{
					NewSlotSlot->SetRow(Row);
					NewSlotSlot->SetColumn(Column);
					int32 LocAddedContainerSlot = InventoryUI->ContainerWidgetComp->ContainerSlots.Add(NewSlot);
					InventoryUI->ContainerWidgetComp->ContainerSlots[LocAddedContainerSlot]->bIsStorageSlot = bIsStorage;
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("New Slot Slot Casting Failed On Inventory Manager Component"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("New Slot Not Set"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Container Widget Comp Not Set"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
}

FItemInformation UInventoryManagerComponent::GetHotbarSlotItem(int32 HotbarSlot)
{
	if (InventoryUI && InventoryUI->HotbarWidgetComp)
	{
		if (HotbarSlot >= 0 && HotbarSlot < InventoryUI->HotbarWidgetComp->HotbarSlots.Num())
		{
			if (InventoryUI->HotbarWidgetComp->HotbarSlots[HotbarSlot])
			{
				return InventoryUI->HotbarWidgetComp->HotbarSlots[HotbarSlot]->HotbarItemInformation;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("HotbarSlots %d Is Invalid"), HotbarSlot);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HotbarSlot index %d is out of bounds"), HotbarSlot);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
	return FItemInformation();
}

void UInventoryManagerComponent::SetHotbarSlotItem(int32 HotbarSlot, FItemInformation ItemInfo)
{
	if (InventoryUI)
	{
		if (InventoryUI->HotbarWidgetComp)
		{
			InventoryUI->HotbarWidgetComp->HotbarSlots[HotbarSlot]->HotbarItemInformation.ID = ItemInfo.ID;
			InventoryUI->HotbarWidgetComp->HotbarSlots[HotbarSlot]->HotbarItemInformation.Name = ItemInfo.Name;
			InventoryUI->HotbarWidgetComp->HotbarSlots[HotbarSlot]->HotbarItemInformation.Amount = ItemInfo.Amount;
			InventoryUI->HotbarWidgetComp->HotbarSlots[HotbarSlot]->HotbarItemInformation.Icon = ItemInfo.Icon;
			InventoryUI->HotbarWidgetComp->HotbarSlots[HotbarSlot]->HotbarItemInformation.Quality = ItemInfo.Quality;
			InventoryUI->HotbarWidgetComp->HotbarSlots[HotbarSlot]->HotbarItemInformation.Type = ItemInfo.Type;

			// UE_LOG(LogTemp, Error, TEXT("HotbarSlots %s Is Valid"), *InventoryUI->HotbarWidgetComp->HotbarSlots[HotbarSlot]->HotbarItemInformation.ID.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Hotbar UI Not Set"));
		}

		UE_LOG(LogTemp, Error, TEXT("Inventory UI Is Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
}

void UInventoryManagerComponent::ClearHotbarSlotItem(int32 HotbarSlot)
{
	if (InventoryUI)
	{
		if (InventoryUI->HotbarWidgetComp)
		{
			InventoryUI->HotbarWidgetComp->HotbarSlots[HotbarSlot]->HotbarItemInformation = FItemInformation();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Hotbar UI Not Set"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory UI Not Set"));
	}
}

void UInventoryManagerComponent::MoveHotbarSlotItem(int32 FromSlot, int32 ToHotBarSlot, bool bFromInventory, bool bFromHotbar)
{
	UE_LOG(LogTemp, Warning, TEXT("From Hotbar Number : %d"), FromSlot);
	UE_LOG(LogTemp, Warning, TEXT("To Hotbar Number : %d"), ToHotBarSlot);
	//UE_LOG(LogTemp, Warning, TEXT("From Inventory : %d"), bFromInventory);
	if (bFromInventory)
	{
		FItemInformation ItemInfo = GetInventorySlotItem(FromSlot);
		SetHotbarSlotItem(ToHotBarSlot, ItemInfo);

		//UE_LOG(LogTemp, Error, TEXT("Move Hotbar From Inventory"));
	}
	else if (bFromHotbar)
	{
		//UE_LOG(LogTemp, Error, TEXT("Move Hotbar From Hotbar"));

		FItemInformation FromItemInfo = GetHotbarSlotItem(FromSlot);
		FItemInformation SwapItemInfo = GetHotbarSlotItem(ToHotBarSlot);

		UE_LOG(LogTemp, Error, TEXT("From Hotbar Item Infor : %s"), *FromItemInfo.ID.ToString());

		// Is Swapping On Hotbar
		if (SwapItemInfo.Icon)
		{
			SetHotbarSlotItem(ToHotBarSlot, FromItemInfo);
			SetHotbarSlotItem(FromSlot, SwapItemInfo);

			// UE_LOG(LogTemp, Error, TEXT("SwapItemInfo Is Valid"));
		}
		else
		{
			//UE_LOG(LogTemp, Error, TEXT("SwapItemInfo Invalid"));
			//UE_LOG(LogTemp, Error, TEXT("HotbarSlots %s Is Valid"), *FromItemInfo.ID.ToString());
			SetHotbarSlotItem(ToHotBarSlot, FromItemInfo);
			ClearHotbarSlotItem(FromSlot);
		}
	}
}

void UInventoryManagerComponent::UseHotbarSlot(int32 HotbarSlot)
{
	FItemInformation LocalItemInfo = GetHotbarSlotItem(HotbarSlot);
	UE_LOG(LogTemp, Error, TEXT("Hotbar Slot %d with name :%s"), HotbarSlot, *LocalItemInfo.ID.ToString());
	if (LocalItemInfo.Icon)
	{
		//UE_LOG(LogTemp, Error, TEXT("Hotbar Slot Item Valid"));
		if (InventoryUI)
		{
			int32 LocalInventorySlotIndex = 0;
			bool bLocalSucces = false;
			for (int32 i = 0; i < InventoryUI->InventorySlotWidgets.Num() - 1; i++)
			{
				//UE_LOG(LogTemp, Error, TEXT("Inventory Slot %d with name :%s"), i, *InventoryUI->InventorySlotWidgets[i]->InvSlotItemInformation.ID.ToString());
				if (InventoryUI->InventorySlotWidgets[i]->InvSlotItemInformation.ID == LocalItemInfo.ID)
				{
					//UE_LOG(LogTemp, Error, TEXT("Hotbar Slot Item Has Found In Inventory"));
					LocalInventorySlotIndex = i;
					bLocalSucces = true;
					break;
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("Hotbar Slot Item Not Found In Inventory"));
				}
			}
			if (bLocalSucces)
			{
				//UE_LOG(LogTemp, Error, TEXT("Use Inventory Item"));
				UseInventoryItem(LocalInventorySlotIndex);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Hotbar Slot Item Not Valid"));
	}
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

	// Log item yang akan ditambahkan
	UE_LOG(LogTemp, Log, TEXT("Trying to add item: %s, Amount: %d"), *Item.ID.ToString(), LocItemAmount);

	// Jika item adalah currency, tambahkan ke gold
	if (Item.ItemType == EItemType::Currency)
	{
		AddGold(LocItemAmount);
		return true;
	}

	// Jika item adalah stackable
	if (Item.bIsStackable)
	{
		// Coba tambahkan ke stack yang ada
		LocItemAmount = FindAndAddAmountToStacks(Inventory, LocItemID, LocItemAmount);

		// Jika tidak ada sisa item yang perlu ditambahkan, kita bisa mengembalikan true
		if (LocItemAmount == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("All items added to stacks, no remaining amount to add."));
			return true;
		}
		else
		{
			// Set jumlah item yang tersisa
			SetItemAmount(Item, LocItemAmount);
		}
	}

	// Pastikan inventory valid sebelum melanjutkan
	if (Inventory)
	{
		int32 IndexEmptySlot = 0;
		// Cek apakah ada slot kosong di inventory
		if (Inventory->GetEmptyInventorySpace(IndexEmptySlot))
		{
			// Tambahkan item ke slot kosong
			AddItem(Inventory, IndexEmptySlot, Item);
			UE_LOG(LogTemp, Log, TEXT("Item added to inventory at slot: %d"), IndexEmptySlot);
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Inventory Full"));
			return false;
		}
	}

	// Jika inventory tidak valid
	UE_LOG(LogTemp, Error, TEXT("Inventory is not valid."));
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

void UInventoryManagerComponent::UseInventoryItem(int32 InvSlot, int32 HotbarSlot)
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
			// UE_LOG(LogTemp, Error, TEXT("Use Consumable Item"));
			UseConsumableItem(InvSlot, HotbarSlot, LocalInvItem);
			break;
		default:
			break;
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

void UInventoryManagerComponent::UseConsumableItem(int32 InvSlot, int32 HotbarSlot, FInventoryItem InvItem)
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
				UE_LOG(LogTemp, Error, TEXT("fully removed"));
				RemoveItem(PlayerInventory, InvSlot);
				ClearHotbarSlotItem(HotbarSlot);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("not full removed"));
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
	GEngine->AddOnScreenDebugMessage(
		-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
		5.0f,                 // Durasi pesan dalam detik
		FColor::Green,        // Warna pesan
		FString::Printf(TEXT("Equip Item")) // Pesan
	);
	if (FromInv && ToInv)
	{
		if (FromInv == ToInv && FromInvSlot == ToInvSlot)
		{
			// Do Nothing
		}
		else
		{
			FInventoryItem LocInvItem = FromInv->GetInventoryItem(FromInvSlot);
			EEquipmentSlotsType LocEquipmentSlotType = ItemEquipSlot(LocInvItem);
			if (ItemType(LocInvItem) == EItemType::Equipment)
			{
				if (GetEquipmentTypeBySlot(ToInvSlot) == LocEquipmentSlotType)
				{
					FInventoryItem LocSwapInvItem = ToInv->GetInventoryItem(ToInvSlot);
					// Swap Item?
					if (ItemIsValid(LocSwapInvItem))
					{
						// Can The Swap Destination Inventory Store Item
						if (CanContainerStoreItem(FromInv))
						{
							if (GEngine)
							{
								GEngine->AddOnScreenDebugMessage(
									-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
									5.0f,                 // Durasi pesan dalam detik
									FColor::Green,        // Warna pesan
									FString::Printf(TEXT("Swap Item")) // Pesan
								);
							}

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
						if (GEngine)
						{
							GEngine->AddOnScreenDebugMessage(
								-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
								5.0f,                 // Durasi pesan dalam detik
								FColor::Green,        // Warna pesan
								FString::Printf(TEXT("Move Item")) // Pesan
							);
						}

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
							FString::Printf(TEXT("Item Can Not Be equipped In That Slot")) // Pesan
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


//void UInventoryManagerComponent::EquipItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot)
//{
//	if (FromInv && ToInv)
//	{
//		if (FromInv != ToInv && FromInvSlot != ToInvSlot)
//		{
//			FInventoryItem LocInvItem = FromInv->GetInventoryItem(FromInvSlot);
//			EEquipmentSlotsType LocEquipmentSlotType = ItemEquipSlot(LocInvItem);
//			if (ItemType(LocInvItem) == EItemType::Equipment)
//			{
//				if (GetEquipmentTypeBySlot(ToInvSlot) == LocEquipmentSlotType)
//				{
//					FInventoryItem LocSwapInvItem = FromInv->GetInventoryItem(FromInvSlot);
//					// Swap Item?
//					if (ItemIsValid(LocSwapInvItem))
//					{
//						// Can The Swap Destination Inventory Store Item
//						if (CanContainerStoreItem(FromInv))
//						{
//							// Swap Items
//							AddItem(ToInv, ToInvSlot, LocInvItem);
//							AddItem(FromInv, FromInvSlot, LocSwapInvItem);
//						}
//						else
//						{
//							if (GEngine)
//							{
//								GEngine->AddOnScreenDebugMessage(
//									-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
//									5.0f,                 // Durasi pesan dalam detik
//									FColor::Green,        // Warna pesan
//									FString::Printf(TEXT("CONTAINER CANNOT STORE ITEMS")) // Pesan
//								);
//							}
//						}
//					}
//					else
//					{
//						// Move item
//						AddItem(ToInv, ToInvSlot, LocInvItem);
//						RemoveItem(FromInv, FromInvSlot);
//					}
//					UpdateEquippedStats();
//				}
//				else
//				{
//					if (GEngine)
//					{
//						GEngine->AddOnScreenDebugMessage(
//							-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
//							5.0f,                 // Durasi pesan dalam detik
//							FColor::Green,        // Warna pesan
//							FString::Printf(TEXT("Item Can Not Be equippped In That Slot")) // Pesan
//						);
//					}
//				}
//			}
//			else
//			{
//				if (GEngine)
//				{
//					GEngine->AddOnScreenDebugMessage(
//						-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
//						5.0f,                 // Durasi pesan dalam detik
//						FColor::Green,        // Warna pesan
//						FString::Printf(TEXT("Item Is Not Equippable")) // Pesan
//					);
//				}
//			}
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("Inventory Not Set"));
//	}
//}

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
	if (InventoryUI)
	{
		if (InventoryUI->InventoryWidgetComp)
		{
			InventoryUI->InventoryWidgetComp->InventoryVisibility = ESlateVisibility::Visible;
			bIsInventoryOpen = true;

			// UE_LOG(LogTemp, Error, TEXT("InventoryVisibility Is Visible"));
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

void UInventoryManagerComponent::CloseInventoryWindow()
{
	if (InventoryUI)
	{
		if (InventoryUI->InventoryWidgetComp)
		{
			InventoryUI->InventoryWidgetComp->InventoryVisibility = ESlateVisibility::Hidden;
			bIsInventoryOpen = false;
			int32 StartIndex = GetNumberOfEquipmentSlots();
			int32 EndIndex = InventoryUI->InventorySlotWidgets.Num() - 1;
			if (StartIndex > EndIndex) {
				std::swap(StartIndex, EndIndex);
			}
			for (int32 i = StartIndex; i < EndIndex; i++)
			{
				InventoryUI->InventorySlotWidgets[i]->bIsSlotHovered = false;
				InventoryUI->InventorySlotWidgets[i]->bIsRightMouseButtonDown = false;
			}

			UE_LOG(LogTemp, Error, TEXT("InventoryVisibility Is Hiding"));
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

void UInventoryManagerComponent::OpenContainerWindow()
{
	if (InventoryUI)
	{
		if (InventoryUI->ContainerWidgetComp)
		{
			InventoryUI->ContainerWidgetComp->ContainerVisibility = ESlateVisibility::Visible;
			bIsContainerOpen = true;
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

void UInventoryManagerComponent::CloseContainerWindow()
{
	if (InventoryUI)
	{
		if (InventoryUI->ContainerWidgetComp)
		{
			InventoryUI->ContainerWidgetComp->ContainerVisibility = ESlateVisibility::Hidden;
			bIsContainerOpen = false;
			for (UContainerSlotWidget* ContSlotWidget : InventoryUI->ContainerWidgetComp->ContainerSlots)
			{
				ContSlotWidget->bIsSlotHovered = false;
				ContSlotWidget->bIsRightMouseButtonDown = false;
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

void UInventoryManagerComponent::OpenEquipmentWindow()
{
	if (InventoryUI)
	{
		if (InventoryUI->EquipmentWidgetComp)
		{
			InventoryUI->EquipmentWidgetComp->EquipmentVisibility = ESlateVisibility::Visible;
			bIsEquipmentOpen = true;
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

void UInventoryManagerComponent::CloseEquipmentWindow()
{
	if (InventoryUI)
	{
		if (InventoryUI->EquipmentWidgetComp)
		{
			InventoryUI->EquipmentWidgetComp->EquipmentVisibility = ESlateVisibility::Hidden;
			bIsEquipmentOpen = false;
			for (int32 i = 0; i < GetNumberOfEquipmentSlots() - 1; i++)
			{
				InventoryUI->InventorySlotWidgets[i]->bIsSlotHovered = false;
				InventoryUI->InventorySlotWidgets[i]->bIsRightMouseButtonDown = false;
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

	// Loop melalui semua item di inventory
	for (FInventoryItem& InvItem : Inventory->GetInventoryItems())
	{
		// Log ID yang sedang dibandingkan
		//UE_LOG(LogTemp, Log, TEXT("Comparing ItemID: %s with InvItem.ID: %s"), *ItemID.ToString(), *InvItem.ID.ToString());

		// Cek apakah item cocok dan stackable
		if (InvItem.ID == ItemID && InvItem.bIsStackable)
		{
			//UE_LOG(LogTemp, Log, TEXT("Found stackable item: %s at slot %d"), *InvItem.ID.ToString(), LocInvSlot);

			// Tambahkan item ke stack
			Amount = AddItemToStack(Inventory, LocInvSlot, Amount);

			// Jika tidak ada sisa item yang perlu ditambahkan, kembalikan 0
			if (Amount == 0)
			{
				//UE_LOG(LogTemp, Log, TEXT("All items added to stack. Remaining amount: %d"), Amount);
				return Amount; // Semua item telah ditambahkan ke stack
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("Remaining amount after adding to stack: %d"), Amount);
			}
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("Item at slot %d is not stackable or does not match ID: %s"), LocInvSlot, *InvItem.ID.ToString());
		}

		LocInvSlot++; // Pindah ke slot berikutnya
	}

	// Kembalikan jumlah sisa yang tidak dapat ditambahkan ke stack
	UE_LOG(LogTemp, Log, TEXT("No more stackable items found. Remaining amount: %d"), Amount);
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

		// Log informasi awal
		//UE_LOG(LogTemp, Log, TEXT("Adding %d items to slot %d."), AmountToAdd, InvSlot);
		//UE_LOG(LogTemp, Log, TEXT("Current amount: %d, Max stack size: %d"), LocItemAmount, LocInvItem.MaxStackSize);
		//UE_LOG(LogTemp, Log, TEXT("ID: %s"), *LocInvItem.ID.ToString());

		// Cek apakah slot sudah ada item
		if (LocItemAmount < LocMaxStackSize)
		{
			int32 FreeStackSpace = LocMaxStackSize - LocItemAmount;

			// Jika jumlah yang ingin ditambahkan lebih kecil atau sama dengan ruang kosong
			if (AmountToAdd <= FreeStackSpace)
			{
				LocRemainingAmount = 0;
				AddItem(Inventory, InvSlot, AddToItemAmount(LocInvItem, AmountToAdd));
				//UE_LOG(LogTemp, Log, TEXT("Added %d items to slot %d. New amount: %d"), AmountToAdd, InvSlot, LocItemAmount + AmountToAdd);
				return LocRemainingAmount;
			}
			else
			{
				LocRemainingAmount = AmountToAdd - FreeStackSpace;
				AddItem(Inventory, InvSlot, AddToItemAmount(LocInvItem, FreeStackSpace));
				//UE_LOG(LogTemp, Log, TEXT("Added %d items to slot %d. New amount: %d. Remaining amount: %d"), FreeStackSpace, InvSlot, LocItemAmount + FreeStackSpace, LocRemainingAmount);
				return LocRemainingAmount;
			}
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("Slot %d is full. Cannot add more items."), InvSlot);
			return LocRemainingAmount; // Slot sudah penuh, kembalikan jumlah yang tersisa
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Not Set"));
		return int32();
	}
}


FInventoryItem UInventoryManagerComponent::SetItemAmount(FInventoryItem& InvItem, int32 AmountToAdd)
{
	InvItem.Amount = AmountToAdd;
	return InvItem;
}

FInventoryItem UInventoryManagerComponent::AddToItemAmount(FInventoryItem& InvItem, int32 AmountToAdd)
{
	InvItem.Amount = InvItem.Amount + AmountToAdd;

	// Log untuk melacak jumlah item yang ditambahkan
	//UE_LOG(LogTemp, Log, TEXT("Adding %d to item amount. New amount: %d"), AmountToAdd, LocInvItem.Amount);

	return InvItem;
}

bool UInventoryManagerComponent::RemoveFromItemAmount(FInventoryItem& OutInvItem, int32& AmountToRemove)
{
	int32 LocAmountRemoved = 0;
	if (AmountToRemove >= ItemAmount(OutInvItem))
	{
		LocAmountRemoved = ItemAmount(OutInvItem);
		AmountToRemove = LocAmountRemoved;
		// UE_LOG(LogTemp, Log, TEXT("Amount To Remove > Item Amount"));
		return true;
	}
	else
	{
		// UE_LOG(LogTemp, Log, TEXT("Amount To Remove < Item Amount"));
		LocAmountRemoved = AmountToRemove;
		SetItemAmount(OutInvItem, ItemAmount(OutInvItem) - AmountToRemove);
		return false;
	}
}

void UInventoryManagerComponent::AddItem(UInventoryComponent* InvComp, int32 InvSlot, FInventoryItem InvItem)
{
	if (InvComp)
	{
		// Log informasi sebelum menambahkan item
		UE_LOG(LogTemp, Log, TEXT("Setting item at slot %d to %s with amount %d"), InvSlot, *InvItem.ID.ToString(), InvItem.Amount);

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
	// Log untuk memeriksa apakah FromInv dan ToInv valid
	if (FromInv && ToInv)
	{
		UE_LOG(LogTemp, Log, TEXT("Moving item from slot %d to slot %d"), FromInvSlot, ToInvSlot);

		// Cek apakah FromInv dan ToInv adalah inventory yang sama dan slot berbeda
		if (FromInv == ToInv && FromInvSlot == ToInvSlot)
		{
			UE_LOG(LogTemp, Log, TEXT("From Inv Same with To Inv And From SLot Index is same with To Slot Index"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("From and To inventories are different and slots are different."));

			// Cek apakah ToInv dapat menyimpan item
			if (CanContainerStoreItem(ToInv))
			{
				FInventoryItem LocalInvItem = FromInv->GetInventoryItem(FromInvSlot);
				UE_LOG(LogTemp, Log, TEXT("Item to move: %s, Amount: %d"), *LocalInvItem.ID.ToString(), LocalInvItem.Amount);

				// Cek apakah item adalah currency
				if (ItemType(LocalInvItem) == EItemType::Currency)
				{
					UE_LOG(LogTemp, Log, TEXT("Item is currency. Adding gold: %d"), ItemAmount(LocalInvItem));
					AddGold(ItemAmount(LocalInvItem));
					RemoveItem(FromInv, FromInvSlot);
					UE_LOG(LogTemp, Log, TEXT("Removed currency item from slot %d."), FromInvSlot);
				}
				else
				{
					FInventoryItem LocalSwapInvItem = ToInv->GetInventoryItem(ToInvSlot);
					if (ItemIsValid(LocalSwapInvItem))
					{
						UE_LOG(LogTemp, Log, TEXT("Swap item found in target slot: %s"), *LocalSwapInvItem.ID.ToString());

						// Cek apakah item dapat di-stack
						if (ItemIsStackable(LocalSwapInvItem) && ItemFreeStackSpace(LocalSwapInvItem) > 0 && ItemIsSame(LocalInvItem, LocalSwapInvItem))
						{
							UE_LOG(LogTemp, Log, TEXT("Items are stackable. Adding to stack."));
							int32 AmountRemaining = AddItemToStack(ToInv, ToInvSlot, ItemAmount(LocalInvItem));
							if (AmountRemaining > 0)
							{
								// Update jumlah item di slot asal
								SetItemAmount(LocalInvItem, AmountRemaining);
								AddItem(FromInv, FromInvSlot, LocalInvItem);
								UE_LOG(LogTemp, Log, TEXT("Updated original item amount. Remaining: %d"), AmountRemaining);
							}
							else
							{
								// Jika semua item telah dipindahkan, hapus item dari slot asal
								RemoveItem(FromInv, FromInvSlot);
								UE_LOG(LogTemp, Log, TEXT("All items moved to stack. Cleared original slot %d."), FromInvSlot);
							}
						}
						else
						{
							// Swap item
							if (CanContainerStoreItem(FromInv))
							{
								AddItem(ToInv, ToInvSlot, LocalInvItem);
								AddItem(FromInv, FromInvSlot, LocalSwapInvItem);
								UE_LOG(LogTemp, Log, TEXT("Swapped items between slots."));
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("Cannot store items in the target inventory."));
							}
						}
					}
					else
					{
						// Pindahkan item ke slot kosong
						AddItem(ToInv, ToInvSlot, LocalInvItem);
						RemoveItem(FromInv, FromInvSlot);
						UE_LOG(LogTemp, Log, TEXT("Moved item to empty slot %d."), ToInvSlot);
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Target inventory cannot store items."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid inventories provided for moving items."));
	}
}


//void UInventoryManagerComponent::MoveItem(UInventoryComponent* FromInv, int32 FromInvSlot, UInventoryComponent* ToInv, int32 ToInvSlot)
//{
//	if (FromInv && ToInv)
//	{
//		if (FromInv != ToInv && FromInvSlot != ToInvSlot)
//		{
//			if (CanContainerStoreItem(ToInv))
//			{
//				FInventoryItem LocalInvItem = FromInv->GetInventoryItem(FromInvSlot);
//				if (ItemType(LocalInvItem) == EItemType::Currency)
//				{
//					AddGold(ItemAmount(LocalInvItem));
//					RemoveItem(FromInv, FromInvSlot);
//				}
//				else
//				{
//					FInventoryItem LocalSwapInvItem = ToInv->GetInventoryItem(ToInvSlot);
//					if (ItemIsValid(LocalSwapInvItem))
//					{
//						// Check if Items are Stackable
//						if (ItemIsStackable(LocalSwapInvItem) && ItemFreeStackSpace(LocalSwapInvItem) > 0 && ItemIsSame(LocalInvItem, LocalSwapInvItem))
//						{
//							// Add To Stack
//							int32 AmountRemaining = AddItemToStack(ToInv, ToInvSlot, ItemAmount(LocalInvItem));
//							if (AmountRemaining > 0)
//							{
//								// Update The From Stack Amount
//								SetItemAmount(LocalInvItem, AmountRemaining);
//								AddItem(FromInv, FromInvSlot, LocalInvItem);
//							}
//							else
//							{
//								// Full Amoun Was Moved Clear Old Stack
//								RemoveItem(FromInv, FromInvSlot);
//							}
//
//						}
//						else
//						{
//							// Swap Item
//							// We are trying to swap, but can the from inventory store items?
//							if (CanContainerStoreItem(FromInv))
//							{
//								AddItem(ToInv, ToInvSlot, LocalInvItem);
//								AddItem(FromInv, FromInvSlot, LocalSwapInvItem);
//							}
//							else
//							{
//								if (GEngine)
//								{
//									GEngine->AddOnScreenDebugMessage(
//										-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
//										5.0f,                 // Durasi pesan dalam detik
//										FColor::Green,        // Warna pesan
//										FString::Printf(TEXT("Container Can't Store Items")) // Pesan
//									);
//
//								}
//							}
//						}
//					}
//					else
//					{
//						// Move Item
//						AddItem(ToInv, ToInvSlot, LocalInvItem);
//						RemoveItem(FromInv, FromInvSlot);
//					}
//				}
//			}
//		}
//	}
//}

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
