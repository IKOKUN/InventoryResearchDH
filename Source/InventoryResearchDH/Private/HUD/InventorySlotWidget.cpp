// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventorySlotWidget.h"
#include "Controller/IRPlayerController.h"
#include "HUD/DragItem.h"
#include "HUD/DraggedItemWidget.h"
#include "HUD/ToolTipWidget.h"
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
				PlayerController->UI_Use_Inventory_Item(InventorySlot);
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
	}
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsSlotHovered = false;
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Memeriksa apakah tombol mouse kanan ditekan
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bIsRightMouseButtonDown = true;
		return FReply::Handled();
	}

	// Memeriksa apakah tombol mouse kiri ditekan
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return EventReply.NativeReply;
	}


	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Memeriksa apakah tombol mouse kanan ditekan
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bIsRightMouseButtonDown = true;
		return FReply::Unhandled();
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UInventorySlotWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InvSlotItemInformation.Icon)
	{
		bIsSlotHovered = true;
	}
	else
	{
		bIsSlotHovered = false;
	}

	return FReply::Unhandled();
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (InvSlotItemInformation.Icon)
	{
		if (DragItemClass && DraggedItemWidgetClass)
		{
			UDraggedItemWidget* DraggedItemWidgetOp = CreateWidget<UDraggedItemWidget>(GetWorld(), DraggedItemWidgetClass);
			DraggedItemWidgetOp->DraggedItemInformation = InvSlotItemInformation;

			// Membuat operasi drag - drop menggunakan DragClass
			OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(DragItemClass);
			if (OutOperation)
			{
				// Coba casting OutOperation ke UDragWidget untuk mengisi WidgetToDrag
				if (UDragItem* DragItemOp = Cast<UDragItem>(OutOperation))
				{
					DragItemOp->DefaultDragVisual = DraggedItemWidgetOp;
					DragItemOp->DraggedItemInformation = InvSlotItemInformation;  // Set widget yang di-drag
					DragItemOp->DraggedItemSlotIndex = InventorySlot;
					DragItemOp->bIsDraggedFromInventory = false;
					DragItemOp->bIsDraggedFromContainer = false;
					DragItemOp->bIsDraggedFromHotbar = true;
					DragItemOp->Pivot = EDragPivot::CenterCenter;
					DragItemOp->Offset = FVector2D(0.f, 0.f);
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
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
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
		InvSlotItemInformation.Amount <= 1 ? FText() : FText::AsNumber(InvSlotItemInformation.Amount);
	}
	return FText();
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
	if (InventorySlot >= GetNumberOfEquipmentSlots())
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	else
	{
		return ESlateVisibility::Hidden;
	}
}

UUserWidget* UInventorySlotWidget::GetToolTipWidget() const
{
	// Ambil informasi dari PlayerController
	FToolTipInfo ToolTipInfoTemp;
	FInventoryItem ToolTipInfoPlayerController = PlayerController->UI_Get_ToolTip_Info(InvSlotItemInformation.ID);

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
	UE_LOG(LogTemp, Log, TEXT("IconOnMouseButtonDown called via Blueprint Callable."));

	// Panggil NativeOnMouseButtonDown, lalu ubah hasilnya menjadi FEventReply
	FReply NativeReply = NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// Konversi FReply ke FEventReply
	if (NativeReply.IsEventHandled())
	{
		return UWidgetBlueprintLibrary::Handled();
	}
	return UWidgetBlueprintLibrary::Unhandled();
}
