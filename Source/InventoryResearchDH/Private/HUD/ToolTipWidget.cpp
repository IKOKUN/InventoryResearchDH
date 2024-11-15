// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ToolTipWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/QualityColors.h"
#include "HUD/DragWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UToolTipWidget::UToolTipWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ESlateVisibility UToolTipWidget::GetBorderVisibility() const
{
	if (ItemToolTipInfo.Icon)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Hidden;
}

FLinearColor UToolTipWidget::GetBorderColor() const
{
	FLinearColor LocalColor;
	FQualityColors LocalQualityColors;
	switch (ItemToolTipInfo.Quality)
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

ESlateVisibility UToolTipWidget::GetToolTipVisibility() const
{
	if (ItemToolTipInfo.Icon)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Hidden;
}

FText UToolTipWidget::GetNameText() const
{
	return FText::FromName(ItemToolTipInfo.Name);
}

FText UToolTipWidget::GetTypeText() const
{
	FString TypeString;
	if (ItemToolTipInfo.ItemType == EItemType::Equipment)
	{
        TypeString = FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(ItemToolTipInfo.ItemType));
		return FText::FromString(TypeString);
	}
	else
	{
		switch (ItemToolTipInfo.EquipmentType)
		{
		case EEquipmentType::Weapon:
			TypeString = FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(ItemToolTipInfo.EquipmentSlot));
			return FText::FromString(TypeString);
		case EEquipmentType::Armor:
			TypeString = FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(ItemToolTipInfo.EquipmentType));
			return FText::FromString(TypeString);
		case EEquipmentType::Shield:
			TypeString = FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(ItemToolTipInfo.EquipmentType));
			return FText::FromString(TypeString);
		case EEquipmentType::Miscellaneous:
			TypeString = FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(ItemToolTipInfo.EquipmentType));
			return FText::FromString(TypeString);
		default:
			break;
		}
	}
	return FText();
}

FText UToolTipWidget::GetDamageText() const
{
	if (ItemToolTipInfo.Damage > 0)
	{
		FString ObjString = FString::Printf(TEXT("%d Damage"), ItemToolTipInfo.Damage);
		return FText::FromString(ObjString);
	}
	return FText();
}

FText UToolTipWidget::GetArmorText() const
{
	if (ItemToolTipInfo.Armor > 0)
	{
		FString ArmorString = FString::Printf(TEXT("%d Armor"), ItemToolTipInfo.Armor);
		return FText::FromString(ArmorString);
	}
	return FText();
}

FText UToolTipWidget::GetStackSizeText() const
{
	if (ItemToolTipInfo.bIsStackable)
	{
		FString StackSizeString = FString::Printf(TEXT("Max Stack: %d"), ItemToolTipInfo.MaxStackSize);
		return FText::FromString(StackSizeString);
	}
	return FText();
}

FText UToolTipWidget::GetStrengthText() const
{
	if (ItemToolTipInfo.Strength > 0)
	{
		FString StrengthString = FString::Printf(TEXT("+%d Strength"), ItemToolTipInfo.Strength);
		return FText::FromString(StrengthString);
	}
	return FText();
}

FText UToolTipWidget::GetDexterityText() const
{
	if (ItemToolTipInfo.Dexterity > 0)
	{
		FString DexterityString = FString::Printf(TEXT("+%d Dexterity"), ItemToolTipInfo.Dexterity);
		return FText::FromString(DexterityString);
	}
	return FText();
}

FText UToolTipWidget::GetIntelligenceText() const
{
	if (ItemToolTipInfo.Intelligence > 0)
	{
		FString IntelligenceString = FString::Printf(TEXT("+%d Dexterity"), ItemToolTipInfo.Intelligence);
		return FText::FromString(IntelligenceString);
	}
	return FText();
}

FText UToolTipWidget::GetUseText() const
{
	if (ItemToolTipInfo.ItemType == EItemType::Consumable)
	{
		if (ItemToolTipInfo.Duration < 1.f)
		{
			FString UseString = FString::Printf(TEXT("Use: Restores %f.2f Health"), ItemToolTipInfo.Health);
			return FText::FromString(UseString);
		}
		else
		{
			FString UseString = FString::Printf(TEXT("Use: Restores %f.2f health over %f.2f sec."), ItemToolTipInfo.Health, ItemToolTipInfo.Duration);
			return FText::FromString(UseString);
		}
	}
	return FText();
}

FText UToolTipWidget::GetDescriptionText() const
{
	if (ItemToolTipInfo.Description != FString())
	{
		FString DescriptionString = FString::Printf(TEXT("%s"), *ItemToolTipInfo.Description);
		return FText::FromString(DescriptionString);
	}
	return FText();
}

ESlateVisibility UToolTipWidget::GetDamageVisibility() const
{
	if (ItemToolTipInfo.Damage > 0)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetArmorVisibility() const
{
	if (ItemToolTipInfo.Armor > 0)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetStackSizeVisibility() const
{
	if (ItemToolTipInfo.bIsStackable)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetStrengthVisibility() const
{
	if (ItemToolTipInfo.Strength > 0)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetDexterityVisibility() const
{
	if (ItemToolTipInfo.Dexterity > 0)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetIntelligenceVisibility() const
{
	if (ItemToolTipInfo.Intelligence > 0)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetUseVisibility() const
{
	if (ItemToolTipInfo.ItemType == EItemType::Consumable)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetDescriptionVisibility() const
{
	if (ItemToolTipInfo.Description != FString())
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Collapsed;
}

FReply UToolTipWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Memeriksa apakah tombol mouse kiri ditekan
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return EventReply.NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UToolTipWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragWidgetClass)
	{

		FVector2D DragWindowOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
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
