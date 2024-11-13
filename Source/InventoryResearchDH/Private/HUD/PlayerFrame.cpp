#include "HUD/PlayerFrame.h"
#include "HUD/DragWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Controller/IRPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UPlayerFrame::UPlayerFrame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
}

FText UPlayerFrame::GetNameText() const
{
	if (PlayerController)
	{
		return FText::FromName(PlayerController->GetPlayerName());
	}
    return FText();
}

FText UPlayerFrame::GetHealthText() const
{
    if (PlayerController)
    {
		return FText::FromString(FString::Printf(TEXT("%f / %f"), PlayerController->GetPlayerHealth(), PlayerController->GetPlayerMaxHealth()));
    }
    return FText();
}

float UPlayerFrame::GetHealthPercent() const
{
    if (PlayerController)
    {
		return PlayerController->GetPlayerHealth() / PlayerController->GetPlayerMaxHealth();
    }
    return 0.f;
}

FReply UPlayerFrame::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return EventReply.NativeReply;
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UPlayerFrame::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
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

    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}
