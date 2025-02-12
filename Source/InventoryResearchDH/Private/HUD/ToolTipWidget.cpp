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
		//UE_LOG(LogTemp, Warning, TEXT("GetBorderColor: Unknown item quality."));
		break;
	}

	//UE_LOG(LogTemp, Log, TEXT("GetBorderColor: Color set to %s."), *LocalColor.ToString());
	return LocalColor;
}

FText UToolTipWidget::GetTypeText() const
{
	FString TypeString;
	if (ItemToolTipInfo.ItemType == EItemType::Equipment)
	{
		switch (ItemToolTipInfo.EquipmentType)
		{
		case EEquipmentType::Weapon:
			TypeString = FString::Printf(TEXT("Weapon"));
			return FText::FromString(TypeString);
		case EEquipmentType::Armor:
			TypeString = FString::Printf(TEXT("Armor"));
			return FText::FromString(TypeString);
		case EEquipmentType::Shield:
			TypeString = FString::Printf(TEXT("Shield"));
			return FText::FromString(TypeString);
		case EEquipmentType::Miscellaneous:
			TypeString = FString::Printf(TEXT("Miscellaneous"));
			return FText::FromString(TypeString);
		default:
			//UE_LOG(LogTemp, Warning, TEXT("GetTypeText: Unknown EquipmentType."));
			break;
		}
	}
	else if (ItemToolTipInfo.ItemType == EItemType::Consumable)
	{
		TypeString = FString::Printf(TEXT("Consumable"));
		return FText::FromString(TypeString);
	}
	else if (ItemToolTipInfo.ItemType == EItemType::QuestItem)
	{
		TypeString = FString::Printf(TEXT("Quest Item"));
		return FText::FromString(TypeString);
	}
	else
	{
		TypeString = FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(ItemToolTipInfo.ItemType));
		//UE_LOG(LogTemp, Log, TEXT("GetTypeText: ItemType is %s."), *TypeString);
		return FText::FromString(TypeString);
	}
	return FText();
}

FText UToolTipWidget::GetUseText() const
{
	if (ItemToolTipInfo.ItemType == EItemType::Consumable)
	{
		if (ItemToolTipInfo.Duration < 1.f)
		{
			FString UseString = FString::Printf(TEXT("Use: Restores %.2f Health"), ItemToolTipInfo.Health);
			//UE_LOG(LogTemp, Log, TEXT("GetUseText: Restores %.2f Health."), ItemToolTipInfo.Health);
			return FText::FromString(UseString);
		}
		else
		{
			FString UseString = FString::Printf(TEXT("Use: Restores %.2f health over %.2f sec."), ItemToolTipInfo.Health, ItemToolTipInfo.Duration);
			//UE_LOG(LogTemp, Log, TEXT("GetUseText: Restores %.2f health over %.2f sec."), ItemToolTipInfo.Health, ItemToolTipInfo.Duration);
			return FText::FromString(UseString);
		}
	}
	//UE_LOG(LogTemp, Log, TEXT("GetUseText: Item is not consumable."));
	return FText();
}

//ESlateVisibility UToolTipWidget::GetBorderVisibility() const
//{
//	if (ItemToolTipInfo.Icon)
//	{
//		//UE_LOG(LogTemp, Log, TEXT("GetBorderVisibility: Icon is valid."));
//		return ESlateVisibility::SelfHitTestInvisible;
//	}
//	//UE_LOG(LogTemp, Log, TEXT("GetBorderVisibility: Icon is nullptr."));
//	return ESlateVisibility::Hidden;
//}

//ESlateVisibility UToolTipWidget::GetToolTipVisibility() const
//{
//	if (ItemToolTipInfo.Icon)
//	{
//		//UE_LOG(LogTemp, Log, TEXT("GetToolTipVisibility: Icon is valid."));
//		return ESlateVisibility::SelfHitTestInvisible;
//	}
//	//UE_LOG(LogTemp, Log, TEXT("GetToolTipVisibility: Icon is nullptr."));
//	return ESlateVisibility::Hidden;
//}

FText UToolTipWidget::GetNameText() const
{
	//UE_LOG(LogTemp, Log, TEXT("GetNameText: Name is %s."), *ItemToolTipInfo.Name.ToString());
	return FText::FromName(ItemToolTipInfo.Name);
}

FText UToolTipWidget::GetDamageText() const
{
	if (ItemToolTipInfo.Damage > 0)
	{
		FString ObjString = FString::Printf(TEXT("%d Damage"), ItemToolTipInfo.Damage);
		//UE_LOG(LogTemp, Log, TEXT("GetDamageText: Damage is %d."), ItemToolTipInfo.Damage);
		return FText::FromString(ObjString);
	}
	//UE_LOG(LogTemp, Log, TEXT("GetDamageText: No damage."));
	return FText();
}

FText UToolTipWidget::GetArmorText() const
{
	if (ItemToolTipInfo.Armor > 0)
	{
		FString ArmorString = FString::Printf(TEXT("%d Armor"), ItemToolTipInfo.Armor);
		//UE_LOG(LogTemp, Log, TEXT("GetArmorText: Armor is %d."), ItemToolTipInfo.Armor);
		return FText::FromString(ArmorString);
	}
	//UE_LOG(LogTemp, Log, TEXT("GetArmorText: No armor."));
	return FText();
}

FText UToolTipWidget::GetStackSizeText() const
{
	if (ItemToolTipInfo.bIsStackable)
	{
		FString StackSizeString = FString::Printf(TEXT("Max Stack: %d"), ItemToolTipInfo.MaxStackSize);
		//UE_LOG(LogTemp, Log, TEXT("GetStackSizeText: Max Stack is %d."), ItemToolTipInfo.MaxStackSize);
		return FText::FromString(StackSizeString);
	}
	//UE_LOG(LogTemp, Log, TEXT("GetStackSizeText: Item is not stackable."));
	return FText();
}

FText UToolTipWidget::GetStrengthText() const
{
	if (ItemToolTipInfo.Strength > 0)
	{
		FString StrengthString = FString::Printf(TEXT("+%d Strength"), ItemToolTipInfo.Strength);
		//UE_LOG(LogTemp, Log, TEXT("GetStrengthText: Strength is +%d."), ItemToolTipInfo.Strength);
		return FText::FromString(StrengthString);
	}
	//UE_LOG(LogTemp, Log, TEXT("GetStrengthText: No strength."));
	return FText();
}

FText UToolTipWidget::GetDexterityText() const
{
	if (ItemToolTipInfo.Dexterity > 0)
	{
		FString DexterityString = FString::Printf(TEXT("+%d Dexterity"), ItemToolTipInfo.Dexterity);
		//UE_LOG(LogTemp, Log, TEXT("GetDexterityText: Dexterity is +%d."), ItemToolTipInfo.Dexterity);
		return FText::FromString(DexterityString);
	}
	//UE_LOG(LogTemp, Log, TEXT("GetDexterityText: No dexterity."));
	return FText();
}

FText UToolTipWidget::GetIntelligenceText() const
{
	if (ItemToolTipInfo.Intelligence > 0)
	{
		FString IntelligenceString = FString::Printf(TEXT("+%d Intelligence"), ItemToolTipInfo.Intelligence);
		//UE_LOG(LogTemp, Log, TEXT("GetIntelligenceText: Intelligence is +%d."), ItemToolTipInfo.Intelligence);
		return FText::FromString(IntelligenceString);
	}
	//UE_LOG(LogTemp, Log, TEXT("GetIntelligenceText: No intelligence."));
	return FText();
}

FText UToolTipWidget::GetDescriptionText() const
{
	if (!ItemToolTipInfo.Description.IsEmpty())
	{
		FString DescriptionString = FString::Printf(TEXT("%s"), *ItemToolTipInfo.Description);
		//UE_LOG(LogTemp, Log, TEXT("GetDescriptionText: Description is %s."), *ItemToolTipInfo.Description);
		return FText::FromString(DescriptionString);
	}
	//UE_LOG(LogTemp, Log, TEXT("GetDescriptionText: Description is empty."));
	return FText();
}

ESlateVisibility UToolTipWidget::GetBorderVisibility() const
{
	return ESlateVisibility();
}

ESlateVisibility UToolTipWidget::GetDamageVisibility() const
{
	if (ItemToolTipInfo.Damage > 0)
	{
		//UE_LOG(LogTemp, Log, TEXT("GetDamageVisibility: Damage is visible."));
		return ESlateVisibility::SelfHitTestInvisible;
	}
	//UE_LOG(LogTemp, Log, TEXT("GetDamageVisibility: Damage is not visible."));
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetArmorVisibility() const
{
	if (ItemToolTipInfo.Armor > 0)
	{
		//UE_LOG(LogTemp, Log, TEXT("GetArmorVisibility: Armor is visible."));
		return ESlateVisibility::SelfHitTestInvisible;
	}
	//UE_LOG(LogTemp, Log, TEXT("GetArmorVisibility: Armor is not visible."));
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetStackSizeVisibility() const
{
	if (ItemToolTipInfo.bIsStackable)
	{
		//UE_LOG(LogTemp, Log, TEXT("GetStackSizeVisibility: Stack size is visible."));
		return ESlateVisibility::SelfHitTestInvisible;
	}
	//UE_LOG(LogTemp, Log, TEXT("GetStackSizeVisibility: Stack size is not visible."));
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetStrengthVisibility() const
{
	if (ItemToolTipInfo.Strength > 0)
	{
		//UE_LOG(LogTemp, Log, TEXT("GetStrengthVisibility: Strength is visible."));
		return ESlateVisibility::SelfHitTestInvisible;
	}
	//UE_LOG(LogTemp, Log, TEXT("GetStrengthVisibility: Strength is not visible."));
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetDexterityVisibility() const
{
	if (ItemToolTipInfo.Dexterity > 0)
	{
		//UE_LOG(LogTemp, Log, TEXT("GetDexterityVisibility: Dexterity is visible."));
		return ESlateVisibility::SelfHitTestInvisible;
	}
	//UE_LOG(LogTemp, Log, TEXT("GetDexterityVisibility: Dexterity is not visible."));
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetIntelligenceVisibility() const
{
	if (ItemToolTipInfo.Intelligence > 0)
	{
		//UE_LOG(LogTemp, Log, TEXT("GetIntelligenceVisibility: Intelligence is visible."));
		return ESlateVisibility::SelfHitTestInvisible;
	}
	//UE_LOG(LogTemp, Log, TEXT("GetIntelligenceVisibility: Intelligence is not visible."));
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetUseVisibility() const
{
	if (ItemToolTipInfo.ItemType == EItemType::Consumable)
	{
		//UE_LOG(LogTemp, Log, TEXT("GetUseVisibility: Item is consumable."));
		return ESlateVisibility::SelfHitTestInvisible;
	}
	//UE_LOG(LogTemp, Log, TEXT("GetUseVisibility: Item is not consumable."));
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UToolTipWidget::GetDescriptionVisibility() const
{
	if (!ItemToolTipInfo.Description.IsEmpty())
	{
		//UE_LOG(LogTemp, Log, TEXT("GetDescriptionVisibility: Description is visible."));
		return ESlateVisibility::SelfHitTestInvisible;
	}
	//UE_LOG(LogTemp, Log, TEXT("GetDescriptionVisibility: Description is not visible."));
	return ESlateVisibility::Collapsed;
}

void UToolTipWidget::SetToolTipWidget() const
{
	if (NameText)
	{
		NameText->SetText(FText::FromName(ItemToolTipInfo.Name));
		NameText->SetColorAndOpacity(GetBorderColor());
	}

	if (TypeText)
	{
		// Type->SetText(GetTypeText());
		TypeText->SetText(GetTypeText());
		TypeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (DamageText)
	{
		DamageText->SetText(GetNameText());
		DamageText->SetVisibility(GetDamageVisibility());
	}

	if (ArmorText)
	{
		ArmorText->SetText(GetArmorText());
		ArmorText->SetVisibility(GetArmorVisibility());
	}

	if (StackSizeText)
	{
		StackSizeText->SetText(GetStackSizeText());
		StackSizeText->SetVisibility(GetStackSizeVisibility());
	}

	if (StrengthText)
	{
		StrengthText->SetText(GetStrengthText());
		StrengthText->SetVisibility(GetStrengthVisibility());
	}

	if (DexterityText)
	{
		DexterityText->SetText(GetDexterityText());
		DexterityText->SetVisibility(GetDexterityVisibility());
	}

	if (IntelligenceText)
	{
		IntelligenceText->SetText(GetIntelligenceText());
		IntelligenceText->SetVisibility(GetIntelligenceVisibility());
	}

	if (UseText)
	{
		UseText->SetText(GetUseText());
		UseText->SetVisibility(GetUseVisibility());
	}

	if (DescriptionText)
	{
		DescriptionText->SetText(GetDescriptionText());
		DescriptionText->SetVisibility(GetDescriptionVisibility());
	}
}

/*
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
				//UE_LOG(LogTemp, Warning, TEXT("OutOperation bukan merupakan instance dari UDragWidget"));
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
		//UE_LOG(LogTemp, Warning, TEXT("DragClass tidak diatur pada %s"), *GetName());
	}
}
*/