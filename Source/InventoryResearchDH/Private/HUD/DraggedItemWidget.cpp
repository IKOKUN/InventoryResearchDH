// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DraggedItemWidget.h"
#include "Data/ItemQuality.h"
#include "Data/QualityColors.h"

UDraggedItemWidget::UDraggedItemWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UDraggedItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FLinearColor UDraggedItemWidget::GetBorderColor() const
{
	FLinearColor LocalColor;
	FQualityColors LocalQualityColors;
	switch (DraggedItemInformation.Quality)
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

FText UDraggedItemWidget::GetAmountText() const
{
	return FText::FromString(FString::Printf(TEXT("0")));
}
