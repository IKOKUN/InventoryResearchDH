// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryLayoutWidget.h"
#include "HUD/EquipmentWidget.h"
#include "HUD/DragWidget.h"
#include "HUD/DragItem.h"
#include "HUD/InventorySlotWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Controller/IRPlayerController.h"

UInventoryLayoutWidget::UInventoryLayoutWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UInventoryLayoutWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());

	// Bind Widget
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_0);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_1);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_2);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_3);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_4);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_5);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_6);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_7);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_8);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_9);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_10);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_11);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_12);
	InventorySlotWidgets.Add(EquipmentWidgetComp->InventorySlot_13);
}

bool UInventoryLayoutWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	int32 LocalDraggedSlot = 0;
	UDragWidget* InvDragWidget = Cast<UDragWidget>(InOperation);
	if (InvDragWidget)
	{
		// Hitung offset dari posisi layar ke posisi lokal
		DragWindowOffset = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition()) - InvDragWidget->DropWindowOffset;

		UCanvasPanelSlot* CanvPanelSlot = CanvasPanel->AddChildToCanvas(InvDragWidget->WidgetToDrag);
		if (CanvPanelSlot)
		{
			CanvPanelSlot->SetSize(InvDragWidget->WidgetToDrag->GetDesiredSize());
			CanvPanelSlot->SetPosition(DragWindowOffset);
		}

		// Tambahkan logika Anda di sini (misalnya, simpan offset, update widget, dll.)
		UE_LOG(LogTemp, Log, TEXT("Dragged Widget Offset: %s"), *DragWindowOffset.ToString());

		return true;
	}
	UDragItem* InvDragItem = Cast<UDragItem>(InOperation);
	if (InvDragItem)
	{
		LocalDraggedSlot = InvDragItem->DraggedItemSlotIndex;
		if (InvDragItem->bIsDraggedFromInventory)
		{
			if (PlayerController)
			{
				PlayerController->UI_Drop_Inventory_Item(LocalDraggedSlot);
			}
			return true;
		}
		else if (InvDragItem->bIsDraggedFromContainer)
		{
			return true;
		}
		else if (InvDragItem->bIsDraggedFromHotbar)
		{
			if (PlayerController)
			{
				PlayerController->UI_Clear_Hotbar_Item(LocalDraggedSlot);
			}
			return true;
		}
		else
		{
			PlayerController->UI_Drop_Inventory_Item(LocalDraggedSlot);
			return false;
		}
	}
	return false;
}
