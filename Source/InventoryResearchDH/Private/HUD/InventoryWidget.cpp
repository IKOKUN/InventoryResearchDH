#include "HUD/InventoryWidget.h"
#include "HUD/DragItem.h"
#include "HUD/DragWidget.h"
#include "Controller/IRPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer) // Fix for Problem 1 and Problem 2
{

}

void UInventoryWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct(); // Fix for Problem 3

	PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
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
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);  // Pemanggilan Super di akhir
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
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent); // Fix for Problem 6

	if (PlayerController)
	{
		PlayerController->UI_Set_IsMouseOverUI(true);
	}
}

void UInventoryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent); // Fix for Problem 7

	if (PlayerController)
	{
		PlayerController->UI_Set_IsMouseOverUI(false);
	}
}
