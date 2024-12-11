// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HotbarWidget.h"
#include "HUD/HotbarSlotWidget.h"
#include "HUD/DragItem.h"
#include "HUD/DragWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UHotbarWidget::UHotbarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHotbarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HotbarSlots.Add(HotbarSlot_0);
	HotbarSlots.Add(HotbarSlot_1);
	HotbarSlots.Add(HotbarSlot_2);
	HotbarSlots.Add(HotbarSlot_3);
	HotbarSlots.Add(HotbarSlot_4);
	HotbarSlots.Add(HotbarSlot_5);
	HotbarSlots.Add(HotbarSlot_6);
	HotbarSlots.Add(HotbarSlot_7);
}

//FReply UHotbarWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	// Logika khusus Anda
//	if (bIsHotbarLocked)
//	{
//		return FReply::Handled();  // Menangani event secara langsung
//	}
//	else
//	{
//		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
//		{
//			// Lakukan deteksi drag
//			FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
//			return EventReply.NativeReply;  // Mengembalikan reply yang benar
//		}
//	}
//
//	// Panggil fungsi induk di akhir
//	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);  // Pemanggilan Super di akhir
//}
//
//bool UHotbarWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
//{
//	if (UDragItem* DragHotbarItem = Cast<UDragItem>(InOperation))
//	{
//		if (DragHotbarItem->bIsDraggedFromHotbar)
//		{
//			return false;
//		}
//		else
//		{
//			return true;
//		}
//	}
//	// Panggil fungsi induk di akhir
//	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);  // Pemanggilan Super di akhir
//}
//
//void UHotbarWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
//{
//	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
//
//	FVector2D DragWindowOffset = FVector2D::ZeroVector;
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
