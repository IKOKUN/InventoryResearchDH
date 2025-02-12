#include "HUD/InventoryWidget.h"

#include "HUD/InventorySlotWidget.h"
#include "HUD/DragItem.h"
#include "HUD/DragWidget.h"
#include "Controller/IRPlayerController.h"
#include "InventoryComponent/EquipmentInventoryComponent.h"
#include "InventoryComponent/InventoryManagerComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer) // Fix for Problem 1 and Problem 2
{
	//SortingInventoryMap.Add("Sort By", InventorySlotWidgets);
}

void UInventoryWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct(); // Fix for Problem 3

	PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
	

	if (PlayerController)
	{
		CachedSortingInventoryMap.Add("Sort By", TArray<FInventoryItem>());
		CachedSortingInventoryMap.Add("Rarerity : High To Low", TArray<FInventoryItem>());
		CachedSortingInventoryMap.Add("Rarerity : Low To High", TArray<FInventoryItem>());
		CachedSortingInventoryMap.Add("Newest", TArray<FInventoryItem>());
		CachedSortingInventoryMap.Add("Oldest", TArray<FInventoryItem>());

		// 
		CachedFilterInventoryMap.Add("Filter : All", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Miscellaneous", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Equipment", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Consumable", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Currency", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Quest Item", TArray<FInventoryItem>());

		CachedFilterInventoryMap.Add("Armor", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Weapon", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Shield", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Miscellaneous", TArray<FInventoryItem>());

		CachedFilterInventoryMap.Add("Head", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Shoulder", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Chest", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Hands", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Legs", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Feet", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Back", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Waist", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Accessory", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Earring", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Ring", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Trinket", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Main-Hand", TArray<FInventoryItem>());
		CachedFilterInventoryMap.Add("Off-Hand", TArray<FInventoryItem>());

	}

	if (SortingInventoryComboBox)
	{
		// Mengikat event "OnSelectionChanged"
		SortingInventoryComboBox->OnSelectionChanged.AddDynamic(this, &UInventoryWidget::OnSortingSelectionChanged);
	}

	if (FilterInventoryComboBox)
	{
		FilterInventoryComboBox->OnSelectionChanged.AddDynamic(this, &UInventoryWidget::OnFilterSelectionChanged);
	}
}

void UInventoryWidget::OnSortingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (PlayerController)
	{
		if (SelectedItem == "Sort By")
		{
			if (CachedSortingInventoryMap.Contains("Sort By"))
			{
				if (CurrentSortingStatus == "Sort By") // Cek apakah status saat ini berbeda
				{
					CachedSortingInventoryMap["Sort By"] = PlayerController->GetPlayerInventoryComponent()->GetInventoryItems();
				}
				PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedSortingInventoryMap["Sort By"]);
				PlayerController->GetInventoryManagerComponent()->RefreshInventorySlots(); 

				/*UE_LOG(LogTemp, Log, TEXT("Inventory Item : Original"));
				int32 SlotIndex = 0;
				for (const FInventoryItem& Item : PlayerController->GetPlayerInventoryComponent()->GetInventoryItems())
				{
					UE_LOG(LogTemp, Log, TEXT("Name: %s"), *Item.ID.ToString());
					UE_LOG(LogTemp, Log, TEXT("Slot Index: %d"), SlotIndex);
					SlotIndex++;
				}
				UE_LOG(LogTemp, Log, TEXT("=="));
				UE_LOG(LogTemp, Log, TEXT("=="));
				UE_LOG(LogTemp, Log, TEXT("Inventory Item : Sort By"));
				SlotIndex = 0;
				for (const FInventoryItem& Item : CachedSortingInventoryMap["Sort By"])
				{
					UE_LOG(LogTemp, Log, TEXT("Name: %s"), *Item.ID.ToString());
					UE_LOG(LogTemp, Log, TEXT("Slot Index: %d"), SlotIndex);
					SlotIndex++;
				}*/

				CurrentSortingStatus = "Sort By";
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Key 'Sort By' not found in CachedSortingInventoryMap!"));
			}
		}
		else if (SelectedItem == "Rarerity : High To Low")
		{
			if (CachedSortingInventoryMap.Contains("Rarerity : High To Low"))
			{
				if (CurrentSortingStatus == "Sort By") // Cek apakah status saat ini berbeda
				{
					CachedSortingInventoryMap["Sort By"] = PlayerController->GetPlayerInventoryComponent()->GetInventoryItems();
				}	

				CachedSortingInventoryMap["Rarerity : High To Low"] = CachedSortingInventoryMap["Sort By"];

				// Sort "Rarerity : High To Low" (Descending order)
				CachedSortingInventoryMap["Rarerity : High To Low"] = SetSortingInventoryMapFromRarerity("Rarerity : High To Low",
					true,
					PlayerController->GetInventoryManagerComponent()->GetNumberOfEquipmentSlots());

				PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedSortingInventoryMap["Rarerity : High To Low"]);
				PlayerController->GetInventoryManagerComponent()->RefreshInventorySlots();
				/*int32 SlotIndex = 0;
				for (const FInventoryItem& Item : CachedSortingInventoryMap["Rarerity : High To Low"])
				{
					UE_LOG(LogTemp, Log, TEXT("Name: %s"), *Item.ID.ToString());
					UE_LOG(LogTemp, Log, TEXT("Slot Index: %d"), SlotIndex);
					SlotIndex++;
				}*/
				CurrentSortingStatus = "Rarerity : High To Low";
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Key 'Rarerity : High To Low' not found in CachedSortingInventoryMap!"));
			}		
		}
		else if (SelectedItem == "Rarerity : Low To High")
		{
			
			if (CachedSortingInventoryMap.Contains("Rarerity : Low To High"))
			{
				if (CurrentSortingStatus == "Sort By") // Cek apakah status saat ini berbeda
				{
					CachedSortingInventoryMap["Sort By"] = PlayerController->GetPlayerInventoryComponent()->GetInventoryItems();
				}

				CachedSortingInventoryMap["Rarerity : Low To High"] = CachedSortingInventoryMap["Sort By"];
				// Sort "Rarerity : Low To High" (Ascending order)
				CachedSortingInventoryMap["Rarerity : Low To High"] = SetSortingInventoryMapFromRarerity("Rarerity : Low To High",
					false,
					PlayerController->GetInventoryManagerComponent()->GetNumberOfEquipmentSlots());

				PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedSortingInventoryMap["Rarerity : Low To High"]);
				PlayerController->GetInventoryManagerComponent()->RefreshInventorySlots();

				/*for (const auto& Pair : CachedSortingInventoryMap)
				{
					UE_LOG(LogTemp, Log, TEXT("Category: %s"), *Pair.Key);

					for (const FInventoryItem& Item : Pair.Value)
					{
						UE_LOG(LogTemp, Log, TEXT("Item Quality: %d"), static_cast<uint8>(Item.Quality));
					}
				}*/
				CurrentSortingStatus = "Rarerity : Low To High";
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Key 'Rarerity : Low To High' not found in CachedSortingInventoryMap!"));
			}
		}
		else if (SelectedItem == "Newest")
		{
			if (CachedSortingInventoryMap.Contains("Newest"))
			{
				//PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedSortingInventoryMap["Newest"]);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Key 'Newest' not found in CachedSortingInventoryMap!"));
			}
		}
		else if (SelectedItem == "Oldest")
		{
			if (CachedSortingInventoryMap.Contains("Oldest"))
			{
				//PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedSortingInventoryMap["Oldest"]);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Key 'Oldest' not found in CachedSortingInventoryMap!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid selection!"));
		}
	}
}

void UInventoryWidget::OnFilterSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (PlayerController)
	{
		// Filter berdasarkan EItemType
		if (SelectedItem == "Filter : All")
		{
			if (CurrentFilterStatus == "Filter : All") // Cek apakah status saat ini berbeda
			{
				CachedFilterInventoryMap["Filter : All"] = PlayerController->GetPlayerInventoryComponent()->GetInventoryItems();
			}

			PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedFilterInventoryMap["Filter : All"]);
			PlayerController->GetInventoryManagerComponent()->RefreshInventorySlots();

			CurrentFilterStatus = "Filter : All";
		}
		// Filter berdasarkan EItemType
		else if (SelectedItem == "Miscellaneous")
		{
			if (CurrentFilterStatus == "Filter : All") // Cek apakah status saat ini berbeda
			{
				CachedFilterInventoryMap["Filter : All"] = PlayerController->GetPlayerInventoryComponent()->GetInventoryItems();
			}
			CachedFilterInventoryMap["Miscellaneous"] = CachedFilterInventoryMap["Filter : All"];

			// Filter inventaris berdasarkan tipe Miscellaneous
			CachedFilterInventoryMap["Miscellaneous"] = FilterInventoryByItemType(
				"Miscellaneous",
				EItemType::Miscellaneous,
				PlayerController->GetInventoryManagerComponent()->GetNumberOfEquipmentSlots());
			

			PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedFilterInventoryMap["Miscellaneous"]);
			PlayerController->GetInventoryManagerComponent()->RefreshInventorySlots();

			CurrentFilterStatus = "Miscellaneous";
		}
		else if (SelectedItem == "Equipment")
		{
			if (CurrentFilterStatus == "Filter : All") // Cek apakah status saat ini berbeda
			{
				CachedFilterInventoryMap["Filter : All"] = PlayerController->GetPlayerInventoryComponent()->GetInventoryItems();
			}
			CachedFilterInventoryMap["Equipment"] = CachedFilterInventoryMap["Filter : All"];

			// Filter inventaris berdasarkan tipe Equipment
			CachedFilterInventoryMap["Equipment"] = FilterInventoryByItemType(
				"Equipment",
				EItemType::Equipment,
				PlayerController->GetInventoryManagerComponent()->GetNumberOfEquipmentSlots());

			PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedFilterInventoryMap["Equipment"]);
			PlayerController->GetInventoryManagerComponent()->RefreshInventorySlots();

			CurrentFilterStatus = "Equipment";
		}
		else if (SelectedItem == "Consumable")
		{
			if (CurrentFilterStatus == "Filter : All") // Cek apakah status saat ini berbeda
			{
				CachedFilterInventoryMap["Filter : All"] = PlayerController->GetPlayerInventoryComponent()->GetInventoryItems();
			}
			CachedFilterInventoryMap["Consumable"] = CachedFilterInventoryMap["Filter : All"];
			// Filter inventaris berdasarkan tipe Consumable
			CachedFilterInventoryMap["Consumable"] = FilterInventoryByItemType(
				"Consumable",
				EItemType::Consumable,
				PlayerController->GetInventoryManagerComponent()->GetNumberOfEquipmentSlots());

			PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedFilterInventoryMap["Consumable"]);
			PlayerController->GetInventoryManagerComponent()->RefreshInventorySlots();

			CurrentFilterStatus = "Consumable";
		}
		else if (SelectedItem == "Quest Item")
		{
			if (CurrentFilterStatus == "Filter : All") // Cek apakah status saat ini berbeda
			{
				CachedFilterInventoryMap["Filter : All"] = PlayerController->GetPlayerInventoryComponent()->GetInventoryItems();
			}
			CachedFilterInventoryMap["Quest Item"] = CachedFilterInventoryMap["Filter : All"];
			// Filter inventaris berdasarkan tipe Consumable
			CachedFilterInventoryMap["Quest Item"] = FilterInventoryByItemType(
				"Quest Item",
				EItemType::QuestItem,
				PlayerController->GetInventoryManagerComponent()->GetNumberOfEquipmentSlots());

			PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedFilterInventoryMap["Quest Item"]);
			PlayerController->GetInventoryManagerComponent()->RefreshInventorySlots();

			CurrentFilterStatus = "Quest Item";
		}
		else
		{
			// Tangani filter atau opsi lain
			UE_LOG(LogTemp, Warning, TEXT("Invalid filter or selection!"));
		}
	}
}



TArray<FInventoryItem> UInventoryWidget::SetSortingInventoryMapFromRarerity(const FString& SortingKey, bool bDescending, int32 SlotThreshold)
{
	TArray<FInventoryItem> SortedInventory;

	// Cek apakah key ada di cache
	if (CachedSortingInventoryMap.Contains(SortingKey))
	{
		// Ambil salinan array inventory dari cache
		TArray<FInventoryItem> InventoryItems = CachedSortingInventoryMap[SortingKey];

		// Jika jumlah item kurang dari atau sama dengan threshold, tidak ada yang perlu disort
		if (InventoryItems.Num() <= SlotThreshold)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough items to sort. Total items: %d, Threshold: %d"), InventoryItems.Num(), SlotThreshold);
			return InventoryItems;
		}

		// Pisahkan equipment (slot 0 sampai SlotThreshold-1) dan backpack (SlotThreshold ke atas)
		TArray<FInventoryItem> EquipmentItems;
		TArray<FInventoryItem> BackpackItems;

		// Equipment items: tetap ambil apa adanya (tidak disort)
		for (int32 i = 0; i < SlotThreshold; ++i)
		{
			EquipmentItems.Add(InventoryItems[i]);
		}

		// Backpack items: filter dan ambil item yang valid (misalnya, nama tidak None)
		for (int32 i = SlotThreshold; i < InventoryItems.Num(); ++i)
		{
			// Filter: hanya ambil item dengan nama valid
			if (!InventoryItems[i].Name.IsNone())
			{
				BackpackItems.Add(InventoryItems[i]);
				UE_LOG(LogTemp, Log, TEXT("Before Sort - Filtered Name: %s, Slot Index: %d, Quality: %d"),
					*InventoryItems[i].Name.ToString(), i, static_cast<uint8>(InventoryItems[i].Quality));
			}
		}

		// Lakukan sorting pada backpack items berdasarkan Quality
		BackpackItems.Sort([bDescending](const FInventoryItem& A, const FInventoryItem& B)
			{
				if (bDescending)
				{
					return static_cast<uint8>(A.Quality) > static_cast<uint8>(B.Quality);
				}
				else
				{
					return static_cast<uint8>(A.Quality) < static_cast<uint8>(B.Quality);
				}
			});

		// Gabungkan kembali equipment items (tidak diubah) dengan backpack items yang telah diurutkan
		SortedInventory.Append(EquipmentItems);
		SortedInventory.Append(BackpackItems);

		return SortedInventory;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Key '%s' not found in CachedSortingInventoryMap!"), *SortingKey);
		return SortedInventory; // Kembalikan array kosong jika key tidak ditemukan
	}
}

TArray<FInventoryItem> UInventoryWidget::FilterInventoryByItemType(const FString& FilterKey, EItemType ItemType, int32 SlotThreshold)
{
	TArray<FInventoryItem> FilteredInventory;

	// Cek apakah key ada di cache
	if (CachedFilterInventoryMap.Contains(FilterKey))
	{
		// Ambil salinan array inventory dari cache
		TArray<FInventoryItem> InventoryItems = CachedFilterInventoryMap[FilterKey];

		// Jika jumlah item kurang dari atau sama dengan threshold, tidak ada yang perlu disort
		if (InventoryItems.Num() <= SlotThreshold)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough items to sort. Total items: %d, Threshold: %d"), InventoryItems.Num(), SlotThreshold);
			return InventoryItems;
		}

		// Pisahkan equipment (slot 0 sampai SlotThreshold-1) dan backpack (SlotThreshold ke atas)
		TArray<FInventoryItem> EquipmentItems;
		TArray<FInventoryItem> BackpackItems;

		// Equipment items: tetap ambil apa adanya (tidak disort)
		for (int32 i = 0; i < SlotThreshold; ++i)
		{
			EquipmentItems.Add(InventoryItems[i]);
		}

		// Backpack items: filter dan ambil item yang valid dan sesuai dengan tipe item
		for (int32 i = SlotThreshold; i < InventoryItems.Num(); ++i)
		{
			// Filter: hanya ambil item dengan nama valid
			if (!InventoryItems[i].Name.IsNone())
			{
				if (InventoryItems[i].ItemType == ItemType)
				{
					BackpackItems.Add(InventoryItems[i]);
					UE_LOG(LogTemp, Log, TEXT("Before Filtered Name: %s, Slot Index: %d, Quality: %d"),
						*InventoryItems[i].Name.ToString(), i, static_cast<uint8>(InventoryItems[i].Quality));
				}
			}
		}

		// Gabungkan kembali equipment items (tidak diubah) dengan backpack items yang telah diurutkan
		FilteredInventory.Append(EquipmentItems);
		FilteredInventory.Append(BackpackItems);

		return FilteredInventory;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Key '%s' not found in CachedSortingInventoryMap!"), *FilterKey);
		return FilteredInventory; // Kembalikan array kosong jika key tidak ditemukan
	}
}
void UInventoryWidget::SetSortingInventoryMapFromNewest()
{
}

void UInventoryWidget::SetSortingInventoryMapFromOldest()
{
}

/*
TArray<FInventoryItem> UInventoryWidget::FilterInventoryByItemType(const TArray<FInventoryItem> Inventory, EItemType ItemType, int32 SlotThreshold)
{
	TArray<FInventoryItem> InventoryTemp;
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (i <= SlotThreshold)
		{
			// Jika index di bawah atau sama dengan threshold, tambahkan item apa adanya
			InventoryTemp.Add(Inventory[i]);
		}
		else if (Inventory[i].ItemType == ItemType)
		{
			// Jika index di atas threshold dan tipe item cocok, tambahkan ke hasil filter
			InventoryTemp.Add(Inventory[i]);
		}
	}

	return InventoryTemp;
}
*/

FText UInventoryWidget::GetGoldText() const
{
	if (PlayerController)
	{
		return FText::FromString(FString::Printf(TEXT("%d"), PlayerController->UI_Get_PlayerStats().Gold)); 
	}
	return FText();
}

ESlateVisibility UInventoryWidget::GetInventoryVisibility() const
{
	return InventoryVisibility;
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Logika khusus Anda
	if (bIsWindowLocked)
	{
		return FReply::Handled();  // Menangani event secara langsung
	}
	else
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			// Lakukan deteksi drag
			FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			return EventReply.NativeReply;  // Mengembalikan reply yang benar
		}
	}

	// Panggil fungsi induk di akhir
	return UUserWidget::NativeOnMouseButtonDown(InGeometry, InMouseEvent);  // Pemanggilan Super di akhir
}

//bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
//{
//	if (UDragItem* InvDragItem = Cast<UDragItem>(InOperation))
//	{
//		if (InventoryVisibility == ESlateVisibility::Visible)
//		{
//			return true;
//		}
//	}
//	return false;
//}

//void UInventoryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Drag Widget In InventoryWidget"));
//	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
//
//	if (DragWidgetClass)
//	{
//		DragWindowOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
//		// Membuat operasi drag - drop menggunakan DragClass
//		OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(DragWidgetClass);
//		if (OutOperation)
//		{
//			// Coba casting OutOperation ke UDragWidget untuk mengisi WidgetToDrag
//			if (UDragWidget* DragWidgetOp = Cast<UDragWidget>(OutOperation))
//			{
//				DragWidgetOp->WidgetToDrag = this;  // Set widget yang di-drag
//				DragWidgetOp->DropWindowOffset = DragWindowOffset;  // Set offset dari mouse ke widget
//			}
//			else
//			{
//				UE_LOG(LogTemp, Warning, TEXT("OutOperation bukan merupakan instance dari UDragWidget"));
//			}
//			// Atur payload atau visual default untuk drag-drop
//			OutOperation->Payload = this;             // Set payload ke widget ini atau objek lainnya
//			OutOperation->DefaultDragVisual = this;   // Atur visual drag ke widget ini atau gambar lain
//			OutOperation->Pivot = EDragPivot::CenterCenter;  // Atur pivot untuk operasi drag-drop
//			OutOperation->Offset = FVector2D(0, 0);  // Atur offset dari mouse ke widget
//
//			RemoveFromParent();  // Hapus widget dari parent
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("DragClass tidak diatur pada %s"), *GetName());
//	}
//}

void UInventoryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UUserWidget::NativeOnMouseEnter(InGeometry, InMouseEvent); // Fix for Problem 6

	if (PlayerController)
	{
		PlayerController->UI_Set_IsMouseOverUI(true);
	}
}

void UInventoryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UUserWidget::NativeOnMouseLeave(InMouseEvent); // Fix for Problem 7

	if (PlayerController)
	{
		PlayerController->UI_Set_IsMouseOverUI(false);
	}
}
