// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InspectWidget.h"
#include "Controller/IRPlayerController.h"
#include "HUD/ToolTipWidget.h"


UInspectWidget::UInspectWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void UInspectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());

	if (InspectToolTipWidget)
	{
		SetToolTipWidget();
	}
}

void UInspectWidget::SetToolTipWidget()
{
	FInventoryItem ToolTipInfoPlayerController = PlayerController->UI_Get_ToolTip_Info(InvSlotItemInformation.ID);

	if (InspectToolTipWidget)
	{
		// Set data tooltip
		InspectToolTipWidget->ItemToolTipInfo.Icon = ToolTipInfoPlayerController.Icon;
		InspectToolTipWidget->ItemToolTipInfo.Name = ToolTipInfoPlayerController.Name;
		InspectToolTipWidget->ItemToolTipInfo.Quality = ToolTipInfoPlayerController.Quality;
		InspectToolTipWidget->ItemToolTipInfo.ItemType = ToolTipInfoPlayerController.ItemType;
		InspectToolTipWidget->ItemToolTipInfo.bIsStackable = ToolTipInfoPlayerController.bIsStackable;
		InspectToolTipWidget->ItemToolTipInfo.MaxStackSize = ToolTipInfoPlayerController.MaxStackSize;
		InspectToolTipWidget->ItemToolTipInfo.Health = ToolTipInfoPlayerController.Health;
		InspectToolTipWidget->ItemToolTipInfo.Duration = ToolTipInfoPlayerController.Duration;
		InspectToolTipWidget->ItemToolTipInfo.EquipmentType = ToolTipInfoPlayerController.EquipmentType;
		InspectToolTipWidget->ItemToolTipInfo.EquipmentSlot = ToolTipInfoPlayerController.EquipmentSlot;
		InspectToolTipWidget->ItemToolTipInfo.Damage = ToolTipInfoPlayerController.Damage;
		InspectToolTipWidget->ItemToolTipInfo.Armor = ToolTipInfoPlayerController.Armor;
		InspectToolTipWidget->ItemToolTipInfo.Strength = ToolTipInfoPlayerController.Strength;
		InspectToolTipWidget->ItemToolTipInfo.Dexterity = ToolTipInfoPlayerController.Dexterity;
		InspectToolTipWidget->ItemToolTipInfo.Intelligence = ToolTipInfoPlayerController.Intelligence;
		InspectToolTipWidget->ItemToolTipInfo.Description = ToolTipInfoPlayerController.Description;

		// Logging untuk memeriksa setiap atribut
		UE_LOG(LogTemp, Log, TEXT("Tooltip Info:"));
		UE_LOG(LogTemp, Log, TEXT(" Tool Tip Name: %s"), *InvSlotItemInformation.Name.ToString());
		UE_LOG(LogTemp, Log, TEXT(" Tool Tip Health: %f"), InspectToolTipWidget->ItemToolTipInfo.Health);
		UE_LOG(LogTemp, Log, TEXT(" Tool Tip Health: %f"), ToolTipInfoPlayerController.Health);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CurrentToolTipWidget Invalid"));
	}
}

