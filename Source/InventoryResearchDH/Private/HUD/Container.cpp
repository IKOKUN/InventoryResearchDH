// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Container.h"
#include "HUD/ContainerSlot.h"
#include "Controller/IRPlayerController.h"
#include "HUD/DragItem.h"
#include "HUD/DragWidget.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/ScrollBox.h"
#include "Components/GridPanel.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UContainer::UContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
}

FText UContainer::GetTitleText() const
{
	FString StringName = ContainerName.ToString();
	StringName = StringName.ToUpper();  // Menyimpan hasil ToUpper() ke dalam variabel StringName
	FText Textname = FText::FromString(StringName);

	return Textname;
}


FReply UContainer::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);  // Pemanggilan Super di akhir
}


bool UContainer::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// Logika khusus Anda untuk memproses drag-and-drop
	if (InOperation)
	{
		UDragItem* DragItemOp = Cast<UDragItem>(InOperation);
		if (DragItemOp)
		{
			// Proses item yang di-drop
			return true;  // Menyatakan bahwa drop diterima
		}
	}

	// Panggil fungsi induk di akhir
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);  // Pemanggilan Super di akhir
}


void UContainer::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragWidgetClass)
	{
		DragWindowOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		// Membuat operasi drag - drop menggunakan DragClass
		OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(DragWidgetClass);
		if (OutOperation)
		{
			// Coba casting OutOperation ke UDragWidget untuk mengisi WidgetToDrag
			if (UDragWidget* DragWidgetOp = Cast<UDragWidget>(OutOperation))
			{
				DragWidgetOp->WidgetToDrag = this;  // Set widget yang di-drag
				DragWidgetOp->DropWindowOffset = DragWindowOffset;  // Set offset dari mouse ke widget
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("OutOperation bukan merupakan instance dari UDragWidget"));
			}
			// Atur payload atau visual default untuk drag-drop
			OutOperation->Payload = this;             // Set payload ke widget ini atau objek lainnya
			OutOperation->DefaultDragVisual = this;   // Atur visual drag ke widget ini atau gambar lain
			OutOperation->Pivot = EDragPivot::CenterCenter;  // Atur pivot untuk operasi drag-drop
			OutOperation->Offset = FVector2D(0, 0);  // Atur offset dari mouse ke widget

			RemoveFromParent();  // Hapus widget dari parent
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DragClass tidak diatur pada %s"), *GetName());
	}
}
