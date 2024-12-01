// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventorySlotWidget.h"
#include "HUD/DragItem.h"
#include "HUD/DraggedItemWidget.h"
#include "HUD/ToolTipWidget.h"
#include "Controller/IRPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Data/ItemQuality.h"
#include "Data/QualityColors.h"

UInventorySlotWidget::UInventorySlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
}

void UInventorySlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsSlotHovered)
	{
		if (bIsRightMouseButtonDown)
		{
			bIsRightMouseButtonDown = false;
			bIsSlotHovered = false;

			if (PlayerController)
			{
				PlayerController->UI_Use_Inventory_Item(InventorySlotIndex);
			}
		}
	}
	else
	{
		bIsRightMouseButtonDown = false;
	}
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (InvSlotItemInformation.Icon)
	{
		bIsSlotHovered = true;

		// Menampilkan tooltip
		if (PlayerController)
		{
			SlotToolTipInfo = GetToolTipWidget(); // Mendapatkan tooltip widget
			if (SlotToolTipInfo)
			{
				// Mendapatkan posisi dan ukuran widget
				FGeometry WidgetGeometry = GetCachedGeometry();
				FVector2D WidgetPosition = WidgetGeometry.GetAbsolutePosition();
				FVector2D WidgetSize = WidgetGeometry.GetLocalSize();

				// Menghitung posisi tooltip
				// FVector2D TooltipPosition = WidgetPosition + FVector2D(WidgetSize.X + 5.0f, WidgetSize.Y / 2.0f - 60.0f); // Offset ke kanan dan sedikit ke atas
				FVector2D TooltipPosition = WidgetPosition + FVector2D(-5.0f, -90.0f); // Offset ke kanan dan sedikit ke atas

				// Menampilkan tooltip di posisi yang dihitung
				SlotToolTipInfo->AddToViewport();
				SlotToolTipInfo->SetPositionInViewport(TooltipPosition);
			}
		}
	}
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsSlotHovered = false;

	// Sembunyikan tooltip
	if (SlotToolTipInfo)
	{
		SlotToolTipInfo->RemoveFromParent(); // Menghapus tooltip dari parent
		SlotToolTipInfo = nullptr; // Reset referensi
	}
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NativeOnMouseButtonDown called"));
	}*/

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Left mouse button down in InventorySlotWidget"));
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

		// Jika drag terdeteksi, kembalikan handled
		if (EventReply.NativeReply.IsEventHandled())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Drag Mouse Succesffulfy Handled"));
			// Pastikan NativeOnDragDetected dipanggil
			return EventReply.NativeReply;
		}
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Right mouse button down in InventorySlotWidget"));
		bIsRightMouseButtonDown = true;
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bIsRightMouseButtonDown = true;
		return FReply::Unhandled();
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UInventorySlotWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InGeometry.IsUnderLocation(InMouseEvent.GetScreenSpacePosition()) && InvSlotItemInformation.Icon)
	{
		bIsSlotHovered = true;
		// UE_LOG(LogTemp, Log, TEXT("Mouse is over the slot."));
	}
	else
	{
		bIsSlotHovered = false;
	}

	return FReply::Unhandled();
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UE_LOG(LogTemp, Log, TEXT("For Drag InventorySlot: %d"), InventorySlotIndex);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Drag Mouse Called"));
	}

	if (InvSlotItemInformation.Icon)
	{
		// UE_LOG(LogTemp, Log, TEXT("Drag detected for item: %s"), *InvSlotItemInformation.Name.ToString());
		if (DragItemClass && DraggedItemWidgetClass)
		{
			UDraggedItemWidget* DraggedItemWidgetOp = CreateWidget<UDraggedItemWidget>(GetWorld(), DraggedItemWidgetClass);
			DraggedItemWidgetOp->DraggedItemInformation = InvSlotItemInformation;

			OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(DragItemClass);
			if (OutOperation)
			{
				if (UDragItem* DragItemOp = Cast<UDragItem>(OutOperation))
				{
					DragItemOp->DefaultDragVisual = DraggedItemWidgetOp;
					DragItemOp->DraggedItemInformation = InvSlotItemInformation;
					DragItemOp->DraggedItemSlotIndex = InventorySlotIndex;
					DragItemOp->bIsDraggedFromInventory = true;
					DragItemOp->bIsDraggedFromContainer = false;
					DragItemOp->bIsDraggedFromHotbar = false;
					DragItemOp->Pivot = EDragPivot::CenterCenter;

					FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
					FVector2D AbsolutePosition = InGeometry.GetAbsolutePosition();
					FVector2D DragOffset = MousePosition - AbsolutePosition;

					
					// DragItemOp->Offset = DragOffset;

					// Log untuk memeriksa nilai
					/*if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Mouse Position: %s, Absolute Position: %s, Drag Offset: %s"), *MousePosition.ToString(), *AbsolutePosition.ToString(), *DragOffset.ToString()));
					}*/
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("OutOperation bukan merupakan instance dari UDragWidget"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DragClass tidak diatur pada %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tidak ada item yang di-drag"));
	}
}



bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop called in InventorySlotWidget"));

	// Cek apakah operasi drag-drop valid
	if (UDragItem* DraggedItem = Cast<UDragItem>(InOperation))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item dropped in InventorySlotWidget"));

		// Lakukan sesuatu dengan item yang dijatuhkan
		int32 LocalDraggedSlotIndex = 0;
		LocalDraggedSlotIndex = DraggedItem->DraggedItemSlotIndex;

		// Cek PlayerController
		if (!PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController is nullptr"));
			return false;
		}

		if (LocalDraggedSlotIndex != 0) // Pastikan untuk menggunakan perbandingan
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Local Dragged Slot: %d"), LocalDraggedSlotIndex));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Inventory Slot Index: %d"), InventorySlotIndex));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Local Dragged Slot is zero or invalid"));
		}
		UE_LOG(LogTemp, Warning, TEXT("Number Of Equipment Slots : %d"), GetNumberOfEquipmentSlots());
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Number Of Equipment Slots : %d"), GetNumberOfEquipmentSlots());

		// Cek apakah item berasal dari inventory
		if (DraggedItem->bIsDraggedFromInventory)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Item dragged from Inventory"));

			if (LocalDraggedSlotIndex < GetNumberOfEquipmentSlots())
			{
				//UE_LOG(LogTemp, Warning, TEXT("UnEquip Item From Inventory"));
				PlayerController->UI_UnEquip_Inventory_Item(LocalDraggedSlotIndex, InventorySlotIndex);
			}
			else if (InventorySlotIndex < GetNumberOfEquipmentSlots())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Equip Item From Inventory"));
				PlayerController->UI_Equip_Inventory_Item(LocalDraggedSlotIndex, InventorySlotIndex);
			}
			else if (PlayerController->UI_Get_IsShiftKeyDown())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Move Item in Inventory (Split)"));
				PlayerController->UI_Split_Inventory_Item(LocalDraggedSlotIndex, InventorySlotIndex, 1);
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Move Item in Inventory"));
				PlayerController->UI_Move_Inventory_Item(LocalDraggedSlotIndex, InventorySlotIndex);
			}

			return true;
		}

		// Cek apakah item berasal dari container
		if (DraggedItem->bIsDraggedFromContainer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item dragged from Container"));

			if (InventorySlotIndex < GetNumberOfEquipmentSlots())
			{
				UE_LOG(LogTemp, Warning, TEXT("Equip Item From Container"));
				PlayerController->UI_Equip_From_Container(LocalDraggedSlotIndex, InventorySlotIndex);
			}
			else if (PlayerController->UI_Get_IsShiftKeyDown())
			{
				UE_LOG(LogTemp, Warning, TEXT("Take Item From Container (Split)"));
				PlayerController->UI_Split_Item_From_Container(LocalDraggedSlotIndex, InventorySlotIndex, 1);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Take Item From Container"));
				PlayerController->UI_Take_Container_Item(LocalDraggedSlotIndex, InventorySlotIndex);
			}

			return true;
		}

		// Jika item tidak berasal dari inventory atau container
		UE_LOG(LogTemp, Warning, TEXT("Dragged Item is not from Inventory or Container"));
		return false;
	}

	// Jika operasi drag-drop tidak valid
	UE_LOG(LogTemp, Warning, TEXT("Invalid DragDropOperation"));
	return false;
}

/*

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop called in InventorySlotWidget"));

	if (UDragItem* DraggedItem = Cast<UDragItem>(InOperation))
	{
		// Logika untuk menangani item yang dijatuhkan
		UE_LOG(LogTemp, Warning, TEXT("Item dropped in InventorySlotWidget"));
		// Lakukan sesuatu dengan item yang dijatuhkan
		return true; // Kembalikan true jika drop berhasil
	}
	//return false; // Kembalikan false jika drop tidak berhasil

	int32 LocalDraggedSlotIndex = 0;
	UDragItem* DragItemInvSlot = Cast<UDragItem>(InOperation);

	// Jika operasi drag-drop tidak valid, langsung return false
	if (!DragItemInvSlot)
		return false;

	LocalDraggedSlotIndex = DragItemInvSlot->DraggedItemSlotIndex;

	// Jika PlayerController tidak valid, log pesan dan langsung return false
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is nullptr"));
		return false;
	}

	// Jika item berasal dari inventory
	if (DragItemInvSlot->bIsDraggedFromInventory)
	{
		if (LocalDraggedSlotIndex < GetNumberOfEquipmentSlots())
		{
			// UnEquip Item From Inventory
			PlayerController->UI_UnEquip_Inventory_Item(LocalDraggedSlotIndex, InventorySlot);
		}
		else if (InventorySlot < GetNumberOfEquipmentSlots())
		{
			// Equip Item From Inventory
			PlayerController->UI_Equip_Inventory_Item(LocalDraggedSlotIndex, InventorySlot);
		}
		else if (PlayerController->UI_Get_IsShiftKeyDown())
		{
			// Move Item in Inventory (Split)
			PlayerController->UI_Split_Inventory_Item(LocalDraggedSlotIndex, InventorySlot, 1);
		}
		else
		{
			// Move Item in Inventory
			PlayerController->UI_Move_Inventory_Item(LocalDraggedSlotIndex, InventorySlot);
		}

		return true;
	}

	// Jika item berasal dari container
	if (DragItemInvSlot->bIsDraggedFromContainer)
	{
		if (InventorySlot < GetNumberOfEquipmentSlots())
		{
			// Equip Item From Container
			PlayerController->UI_Equip_From_Container(LocalDraggedSlotIndex, InventorySlot);
		}
		else if (PlayerController->UI_Get_IsShiftKeyDown())
		{
			// Take Item From Container (Split)
			PlayerController->UI_Split_Item_From_Container(LocalDraggedSlotIndex, InventorySlot, 1);
		}
		else
		{
			// Take Item From Container
			PlayerController->UI_Take_Container_Item(LocalDraggedSlotIndex, InventorySlot);
		}

		return true;
	}

	// Jika item tidak berasal dari inventory atau container
	UE_LOG(LogTemp, Warning, TEXT("Dragged Item is not from Inventory"));
	return false;
}
*/
ESlateVisibility UInventorySlotWidget::GetBorderVisibility() const
{
	if (InvSlotItemInformation.Icon)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	else
	{
		return ESlateVisibility::Hidden;
	}
}
FText UInventorySlotWidget::GetAmountText() const
{
	if (InvSlotItemInformation.Icon)
	{
		// Log jumlah item
		// UE_LOG(LogTemp, Log, TEXT("Item Amount: %d"), InvSlotItemInformation.Amount);

		// Jika jumlah item lebih dari 1, kembalikan teks jumlah, jika tidak, kembalikan teks kosong
		return InvSlotItemInformation.Amount > 1 ? FText::AsNumber(InvSlotItemInformation.Amount) : FText::GetEmpty();
	}

	// Log jika tidak ada ikon
	// UE_LOG(LogTemp, Log, TEXT("No icon found for the item in the slot."));
	return FText::GetEmpty(); // Kembalikan teks kosong jika tidak ada ikon
}
FLinearColor UInventorySlotWidget::GetBorderColor() const
{
	FLinearColor LocalColor;
	FQualityColors LocalQualityColors;
	switch (InvSlotItemInformation.Quality)
	{
	case EItemQuality::Poor:
		LocalColor = LocalQualityColors.Poor;
		break;
	case EItemQuality::Common:
		LocalColor = LocalQualityColors.Common;
		break;
	case EItemQuality::Uncommon:
		LocalColor = LocalQualityColors.Uncommon;
		break;
	case EItemQuality::Rare:
		LocalColor = LocalQualityColors.Rare;
		break;
	case EItemQuality::Epic:
		LocalColor = LocalQualityColors.Epic;
		break;
	case EItemQuality::Legendary:
		LocalColor = LocalQualityColors.Legendary;
		break;
	default:
		break;

	}

	return LocalColor;
}

ESlateVisibility UInventorySlotWidget::GetBackgroundVisibility() const
{
	if (InventorySlotIndex >= GetNumberOfEquipmentSlots())
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	else
	{
		return ESlateVisibility::Hidden;
	}
}

UToolTipWidget* UInventorySlotWidget::GetToolTipWidget() const
{
	// Ambil informasi dari PlayerController
	FToolTipInfo ToolTipInfoTemp;
	FInventoryItem ToolTipInfoPlayerController = PlayerController->UI_Get_ToolTip_Info(InvSlotItemInformation.ID);

	//UE_LOG(LogTemp, Log, TEXT("Inv Slot ID: %s"), *InvSlotItemInformation.ID.ToString());

	// Buat widget tooltip baru
	UToolTipWidget* CurrentToolTipWidget = CreateWidget<UToolTipWidget>(PlayerController, ToolTipWidgetClass);

	if (CurrentToolTipWidget)
	{
		// Set data tooltip
		ToolTipInfoTemp.Icon = InvSlotItemInformation.Icon;
		ToolTipInfoTemp.Name = InvSlotItemInformation.Name;
		ToolTipInfoTemp.Quality = InvSlotItemInformation.Quality;
		ToolTipInfoTemp.ItemType = InvSlotItemInformation.Type;
		ToolTipInfoTemp.bIsStackable = ToolTipInfoPlayerController.bIsStackable;
		ToolTipInfoTemp.MaxStackSize = ToolTipInfoPlayerController.MaxStackSize;
		ToolTipInfoTemp.Health = ToolTipInfoPlayerController.Health;
		ToolTipInfoTemp.Duration = ToolTipInfoPlayerController.Duration;
		ToolTipInfoTemp.EquipmentType = ToolTipInfoPlayerController.EquipmentType;
		ToolTipInfoTemp.EquipmentSlot = ToolTipInfoPlayerController.EquipmentSlot;
		ToolTipInfoTemp.Damage = ToolTipInfoPlayerController.Damage;
		ToolTipInfoTemp.Armor = ToolTipInfoPlayerController.Armor;
		ToolTipInfoTemp.Strength = ToolTipInfoPlayerController.Strength;
		ToolTipInfoTemp.Dexterity = ToolTipInfoPlayerController.Dexterity;
		ToolTipInfoTemp.Intelligence = ToolTipInfoPlayerController.Intelligence;

		// Logging untuk memeriksa setiap atribut
		//UE_LOG(LogTemp, Log, TEXT("Tooltip Info:"));
		//UE_LOG(LogTemp, Log, TEXT(" - Name: %s"), *CurrentToolTipWidget->ItemToolTipInfo.Name.ToString());

		// Assign ToolTipInfoTemp ke CurrentToolTipWidget
		CurrentToolTipWidget->ItemToolTipInfo = ToolTipInfoTemp;

		// Return sebagai UUserWidget*
		return CurrentToolTipWidget;
	}
	return nullptr;
}

FSlateBrush UInventorySlotWidget::GetIconBrush() const
{
	FSlateBrush LocalBrush;
	if (InvSlotItemInformation.Icon)
	{
		LocalBrush.SetResourceObject(InvSlotItemInformation.Icon);
		LocalBrush.SetImageSize(FVector2D(64.0f, 64.0f));
	}
	else
	{
		LocalBrush.SetResourceObject(BackgroundIcon);
		LocalBrush.SetImageSize(FVector2D(64.0f, 64.0f));
	}
	return LocalBrush;
}

FEventReply UInventorySlotWidget::IconOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//SUE_LOG(LogTemp, Log, TEXT("IconOnMouseButtonDown called via Blueprint Callable."));

	// Panggil NativeOnMouseButtonDown, lalu ubah hasilnya menjadi FEventReply
	FReply NativeReply = NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// Konversi FReply ke FEventReply
	if (NativeReply.IsEventHandled())
	{
		UE_LOG(LogTemp, Log, TEXT("Drag detected in IconOnMouseButtonDown."));
		return UWidgetBlueprintLibrary::Handled();
	}
	return UWidgetBlueprintLibrary::Unhandled();
}
