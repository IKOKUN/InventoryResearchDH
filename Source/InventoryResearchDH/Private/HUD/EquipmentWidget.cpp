// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EquipmentWidget.h"
#include "Controller/IRPlayerController.h"
#include "HUD/DragItem.h"
#include "HUD/DragWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"


UEquipmentWidget::UEquipmentWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
}

FText UEquipmentWidget::GetDamageText() const
{
	if (PlayerController)
	{
		return FText::FromString(FString::Printf(TEXT("%d Damage"), PlayerController->UI_Get_PlayerStats().Damage));
	}
	return FText();
}

FText UEquipmentWidget::GetArmorText() const
{
	if (PlayerController)
	{
		return FText::FromString(FString::Printf(TEXT("%d Armor"), PlayerController->UI_Get_PlayerStats().Armor));
	}
	return FText();
}

FText UEquipmentWidget::GetDexterityText() const
{
	if (PlayerController)
	{
		return FText::FromString(FString::Printf(TEXT("%d Dexterity"), PlayerController->UI_Get_PlayerStats().Dexterity));
	}
	return FText();
}

FText UEquipmentWidget::GetIntelligenceText() const
{
	if (PlayerController)
	{
		return FText::FromString(FString::Printf(TEXT("%d Intelligence"), PlayerController->UI_Get_PlayerStats().Intelligence));
	}
	return FText();
}

FText UEquipmentWidget::GetStrengthText() const
{
	if (PlayerController)
	{
		return FText::FromString(FString::Printf(TEXT("%d Strength"), PlayerController->UI_Get_PlayerStats().Strength));
	}
	return FText();
}

ESlateVisibility UEquipmentWidget::GetEquipmentVisibility() const
{
	return EquipmentVisibility;
}

FReply UEquipmentWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

bool UEquipmentWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UDragItem* InvDragItem = Cast<UDragItem>(InOperation))
	{
		if (EquipmentVisibility == ESlateVisibility::Visible)
		{
			return true;
		}
	}
	return false;
}

void UEquipmentWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
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

void UEquipmentWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent); // Fix for Problem 6

	if (PlayerController)
	{
		PlayerController->UI_Set_IsMouseOverUI(true);
	}
}

void UEquipmentWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent); // Fix for Problem 7

	if (PlayerController)
	{
		PlayerController->UI_Set_IsMouseOverUI(false);
	}
}
