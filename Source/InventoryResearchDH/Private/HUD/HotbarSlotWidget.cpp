// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HotbarSlotWidget.h"

#include "Controller/IRPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "HUD/DraggedItemWidget.h"
#include "HUD/DragItem.h"
#include "HUD/ToolTipWidget.h"

UHotbarSlotWidget::UHotbarSlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FSlateBrush UHotbarSlotWidget::GetIconBrush() const
{
	FSlateBrush LocalBrush;
	if (HotbarItemInformation.Icon)
	{
		LocalBrush.SetResourceObject(HotbarItemInformation.Icon);
	}
	else
	{
		LocalBrush.SetResourceObject(BackgroundIcon);
	}
	return LocalBrush;
}

FText UHotbarSlotWidget::GetNumberText() const
{
	FText LocalText = FText::FromString(FString::Printf(TEXT("%d"), (HotBarNumber + 1)));
	return LocalText;
}

UToolTipWidget* UHotbarSlotWidget::GetToolTipWidget() const
{
	if (HotbarItemInformation.Icon)
	{
		AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
		if (PlayerController)
		{
			FInventoryItem LocalInvItem = PlayerController->UI_Get_ToolTip_Info(HotbarItemInformation.ID);
			if (LocalInvItem.Icon)
			{
				UToolTipWidget* ToolTipWidgetOp = CreateWidget<UToolTipWidget>(GetWorld(), ToolTipWidgetClass);
				ToolTipWidgetOp->ItemToolTipInfo.Icon = HotbarItemInformation.Icon;
				ToolTipWidgetOp->ItemToolTipInfo.Name = HotbarItemInformation.Name;
				ToolTipWidgetOp->ItemToolTipInfo.Description = LocalInvItem.Description;
				ToolTipWidgetOp->ItemToolTipInfo.Quality = HotbarItemInformation.Quality;
				ToolTipWidgetOp->ItemToolTipInfo.ItemType = HotbarItemInformation.Type;
				ToolTipWidgetOp->ItemToolTipInfo.bIsStackable = LocalInvItem.bIsStackable;
				ToolTipWidgetOp->ItemToolTipInfo.MaxStackSize = LocalInvItem.MaxStackSize;
				ToolTipWidgetOp->ItemToolTipInfo.Health = LocalInvItem.Health;
				ToolTipWidgetOp->ItemToolTipInfo.Duration = LocalInvItem.Duration;
				ToolTipWidgetOp->ItemToolTipInfo.EquipmentType = LocalInvItem.EquipmentType;
				ToolTipWidgetOp->ItemToolTipInfo.EquipmentSlot = LocalInvItem.EquipmentSlot;
				ToolTipWidgetOp->ItemToolTipInfo.Damage = LocalInvItem.Damage;
				ToolTipWidgetOp->ItemToolTipInfo.Armor = LocalInvItem.Armor;
				ToolTipWidgetOp->ItemToolTipInfo.Strength = LocalInvItem.Strength;
				ToolTipWidgetOp->ItemToolTipInfo.Dexterity = LocalInvItem.Dexterity;
				ToolTipWidgetOp->ItemToolTipInfo.Intelligence = LocalInvItem.Intelligence;

				return ToolTipWidgetOp;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController tidak ditemukan pada %s"), *GetName());
		}
	}
	return nullptr;
}

FEventReply UHotbarSlotWidget::IconOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

FReply UHotbarSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Memeriksa apakah tombol mouse kiri ditekan
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return EventReply.NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UHotbarSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (HotbarItemInformation.Icon)
	{
		SlotToolTipInfo = GetToolTipWidget(); // Mendapatkan tooltip widget
		if (SlotToolTipInfo)
		{
			// Menampilkan tooltip di posisi mouse dengan offset
			FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
			FVector2D TooltipOffset(5.0f, -120.0f); // Offset untuk menempatkan tooltip di sebelah kanan
			SlotToolTipInfo->AddToViewport();
			SlotToolTipInfo->SetPositionInViewport(MousePosition + TooltipOffset);
		}
	}
}

void UHotbarSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	// Sembunyikan tooltip
	if (SlotToolTipInfo)
	{
		SlotToolTipInfo->RemoveFromParent(); // Menghapus tooltip dari parent
		SlotToolTipInfo = nullptr; // Reset referensi
	}
}

bool UHotbarSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UDragItem* DragItemOp = Cast<UDragItem>(InOperation))
	{
		AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
		if (PlayerController)
		{
			PlayerController->UI_Move_Hotbar_Item(DragItemOp->DraggedItemSlotIndex, 
				HotBarNumber, 
				DragItemOp->bIsDraggedFromInventory, 
				DragItemOp->bIsDraggedFromHotbar);

			return true;
		}
	}
	return false;
}

void UHotbarSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (HotbarItemInformation.Icon)
	{
		if (DragItemClass && DraggedItemWidgetClass)
		{
			UDraggedItemWidget* DraggedItemWidgetOp = CreateWidget<UDraggedItemWidget>(GetWorld(), DraggedItemWidgetClass);
			DraggedItemWidgetOp->DraggedItemInformation = HotbarItemInformation;

			// Membuat operasi drag - drop menggunakan DragClass
			OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(DragItemClass);
			if (OutOperation)
			{
				// Coba casting OutOperation ke UDragWidget untuk mengisi WidgetToDrag
				if (UDragItem* DragItemOp = Cast<UDragItem>(OutOperation))
				{
					DragItemOp->DefaultDragVisual = DraggedItemWidgetOp;
					DragItemOp->DraggedItemInformation = HotbarItemInformation;  // Set widget yang di-drag
					DragItemOp->DraggedItemSlotIndex = HotBarNumber;
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
