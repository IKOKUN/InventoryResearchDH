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
	}

	if (SortingInventoryComboBox)
	{
		// Menambahkan item ke ComboBox
		//SortingInventoryComboBox->AddOption(TEXT("Sort By"));
		//SortingInventoryComboBox->AddOption(TEXT("Legendary"));
		//SortingInventoryComboBox->AddOption(TEXT("Epic"));
		//SortingInventoryComboBox->AddOption(TEXT("Rare"));
		//SortingInventoryComboBox->AddOption(TEXT("Uncommon"));
		//SortingInventoryComboBox->AddOption(TEXT("Common"));

		//// Mengatur "Sort By" sebagai pilihan default
		//SortingInventoryComboBox->SetSelectedOption(TEXT("Sort By"));
		// Tambahkan hasil sorting ke CachedSortingInventoryMap

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
				SetSortingInventoryMapFromRarerity("Rarerity : High To Low",
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
				SetSortingInventoryMapFromRarerity("Rarerity : Low To High",
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
				PlayerController->GetPlayerInventoryComponent()->GetInventoryItems(),
				EItemType::Miscellaneous
			);

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
				PlayerController->GetPlayerInventoryComponent()->GetInventoryItems(),
				EItemType::Equipment
			);

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
				PlayerController->GetPlayerInventoryComponent()->GetInventoryItems(),
				EItemType::Consumable
			);

			PlayerController->GetPlayerInventoryComponent()->SetInventoryItems(CachedFilterInventoryMap["Consumable"]);
			PlayerController->GetInventoryManagerComponent()->RefreshInventorySlots();

			CurrentFilterStatus = "Consumable";
		}
		else
		{
			// Tangani filter atau opsi lain
			UE_LOG(LogTemp, Warning, TEXT("Invalid filter or selection!"));
		}
	}
}

void UInventoryWidget::SetSortingInventoryMapFromRarerity(const FString& SortingKey, bool bDescending, int32 SlotThreshold)
{
	if (GEngine)
	{
		float DisplayGamma = GEngine->GetDisplayGamma();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Get Display Gamma: %f"), DisplayGamma));

		if (DisplayGamma != 0.0f) // Bandingkan secara eksplisit dengan 0.0f
		{
			GEngine->DisplayGamma = 2.2f;  // Set gamma ke 2.2f
		}
	}

	// Pastikan key ada di CachedSortingInventoryMap
	if (CachedSortingInventoryMap.Contains(SortingKey))
	{
		// Ambil array FInventoryItem berdasarkan key
		TArray<FInventoryItem>& InventoryItems = CachedSortingInventoryMap[SortingKey];

		// Jika jumlah item kurang dari threshold, tidak ada yang perlu diurutkan
		if (InventoryItems.Num() <= SlotThreshold)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough items to sort. Total items: %d, Threshold: %d"), InventoryItems.Num(), SlotThreshold);
			return;
		}

		// Filter item dari SlotThreshold ke atas
		TArray<FInventoryItem> FilteredItems;
		for (int32 SlotIndex = SlotThreshold; SlotIndex < InventoryItems.Num(); ++SlotIndex)
		{
			FInventoryItem& Item = InventoryItems[SlotIndex];

			// Pastikan item memiliki nama yang valid dan kualitas yang layak
			if (!Item.Name.IsNone()) // Memfilter item dengan nama "None" dan Quality <= 0
			{
				FilteredItems.Add(Item);
				UE_LOG(LogTemp, Log, TEXT("Before Sort - Filtered Name: %s, Slot Index: %d, Quality : %d"), *Item.Name.ToString(), SlotIndex, static_cast<uint8>(Item.Quality));
			}
		}

		// Pastikan FilteredItems sudah terisi dengan item yang benar
		/*UE_LOG(LogTemp, Log, TEXT("Filtered Items (before sorting):"));
		for (int32 i = 0; i < FilteredItems.Num(); ++i)
		{
			UE_LOG(LogTemp, Log, TEXT("Filtered Name: %s, Original Slot Index: %d"), *FilteredItems[i].Name.ToString(), i + SlotThreshold);
		}*/

		// Lakukan sorting pada filtered items
		FilteredItems.Sort([bDescending](const FInventoryItem& A, const FInventoryItem& B)
			{
				// Descending order (high to low quality)
				if (bDescending)
				{
					return static_cast<uint8>(A.Quality) > static_cast<uint8>(B.Quality);
				}
				// Ascending order (low to high quality)
				else
				{
					return static_cast<uint8>(A.Quality) < static_cast<uint8>(B.Quality);
				}
			});

		// Log hasil setelah sorting untuk memverifikasi hasil
		/*UE_LOG(LogTemp, Log, TEXT("After Sort - Filtered Items:"));
		for (int32 i = 0; i < FilteredItems.Num(); ++i)
		{
			UE_LOG(LogTemp, Log, TEXT("Sorted Name: %s, Sorted Slot Index: %d, Quality: %d"), *FilteredItems[i].Name.ToString(), i + SlotThreshold, (int32)FilteredItems[i].Quality);
		}*/

		// Update InventoryItems dengan item yang sudah diurutkan, mulai dari SlotThreshold
		for (int32 SlotIndex = SlotThreshold; SlotIndex < InventoryItems.Num(); ++SlotIndex)
		{
			if (SlotIndex - SlotThreshold < FilteredItems.Num())  // Ensure within bounds of FilteredItems
			{
				InventoryItems[SlotIndex] = FilteredItems[SlotIndex - SlotThreshold];
				//UE_LOG(LogTemp, Log, TEXT("Updated Inventory Name: %s, New Slot Index: %d"), *InventoryItems[SlotIndex].Name.ToString(), SlotIndex);
			}
		}
	}
	else
	{
		// Log jika key tidak ditemukan
		UE_LOG(LogTemp, Warning, TEXT("Key '%s' not found in CachedSortingInventoryMap!"), *SortingKey);
	}
}

void UInventoryWidget::SetSortingInventoryMapFromNewest()
{
}

void UInventoryWidget::SetSortingInventoryMapFromOldest()
{
}

TArray<FInventoryItem> UInventoryWidget::FilterInventoryByItemType(const TArray<FInventoryItem>& Inventory, EItemType ItemType)
{
	TArray<FInventoryItem> FilteredItems;

	for (const FInventoryItem& Item : Inventory)
	{
		if (Item.ItemType == ItemType) // Cocokkan dengan ItemType
		{
			FilteredItems.Add(Item);
		}
	}

	return FilteredItems;
}

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
