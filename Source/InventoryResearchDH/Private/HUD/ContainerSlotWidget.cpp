// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ContainerSlotWidget.h"

#include "Controller/IRPlayerController.h"
#include "HUD/ToolTipWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/ItemQuality.h"
#include "Data/QualityColors.h"

UContainerSlotWidget::UContainerSlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UContainerSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
}

void UContainerSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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
				PlayerController->UI_Use_Container_Item(ContainerSlot);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("IRPlayerController is nullptr"));
			}
		}
	}
	else
	{
		bIsRightMouseButtonDown = false;
	}
}

void UContainerSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (ItemInformation.Icon)
	{
		bIsSlotHovered = true;
	}

}

void UContainerSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsSlotHovered = false;
}

ESlateVisibility UContainerSlotWidget::GetBorderVisibility() const
{
	if (ItemInformation.Icon)
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Hidden;
}

FText UContainerSlotWidget::GetAmountText() const
{
	bool bIsCurrency = ItemInformation.Type == EItemType::Currency;
	if (bIsCurrency || ItemInformation.Amount <= 1)
	{
		return FText();
	}
	else
	{
		return FText::AsNumber(ItemInformation.Amount);
	}
}

FLinearColor UContainerSlotWidget::GetBorderColor() const
{
	FLinearColor LocalColor;
	FQualityColors LocalQualityColors;
	switch (ItemInformation.Quality)
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

ESlateVisibility UContainerSlotWidget::GetNameVisibility() const
{
	if (bIsStorageSlot)
	{
		return ESlateVisibility::Collapsed;
	}
	else
	{
		return ESlateVisibility::Visible;
	}
}

FText UContainerSlotWidget::GetNameText() const
{
	FName ItemName = ItemInformation.Name;
	if (ItemInformation.Type == EItemType::Currency)
	{
		FText AmountNameText = FText::Format(FText::FromString("{Amount} {Name}"), FText::AsNumber(ItemInformation.Amount), FText::FromName(ItemName));
		return AmountNameText;
	}
	return FText::FromName(ItemName);
}

UUserWidget* UContainerSlotWidget::GetToolTipWidget() const
{
	// Ambil informasi dari PlayerController
	FToolTipInfo ToolTipInfoTemp;
	FInventoryItem ToolTipInfoPlayerController = PlayerController->UI_Get_ToolTip_Info(ItemInformation.ID);

	// Buat widget tooltip baru
	UToolTipWidget* CurrentToolTipWidget = CreateWidget<UToolTipWidget>(PlayerController, ToolTipWidgetClass);

	if (CurrentToolTipWidget)
	{
		// Set data tooltip
		ToolTipInfoTemp.Icon = ItemInformation.Icon;
		ToolTipInfoTemp.Name = ItemInformation.Name;
		ToolTipInfoTemp.Quality = ItemInformation.Quality;
		ToolTipInfoTemp.ItemType = ItemInformation.Type;
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

FSlateBrush UContainerSlotWidget::GetIconBrush() const
{
	FSlateBrush LocalBrush;
	if (ItemInformation.Icon)
	{
		LocalBrush.SetResourceObject(ItemInformation.Icon);
		LocalBrush.SetImageSize(FVector2D(64.0f, 64.0f));
	}
	else
	{
		LocalBrush.SetResourceObject(BackgroundIcon);
		LocalBrush.SetImageSize(FVector2D(64.0f, 64.0f));
	}
	return LocalBrush;
}