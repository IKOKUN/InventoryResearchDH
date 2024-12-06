// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InteractSlotWidget.h"
#include "Controller/IRPlayerController.h"
#include "Components/Button.h"

UInteractSlotWidget::UInteractSlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UInteractSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();


    PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());

    // Check if buttons are valid before binding
    if (EquipButton)
    {
        EquipButton->OnClicked.AddDynamic(this, &UInteractSlotWidget::OnEquipButtonClicked);
    }

    if (DropButton)
    {
        DropButton->OnClicked.AddDynamic(this, &UInteractSlotWidget::OnDropButtonClicked);
    }
}

void UInteractSlotWidget::OnEquipButtonClicked()
{
    // Handle Equip button click
    UE_LOG(LogTemp, Log, TEXT("Equip Button Clicked!"));
    // Add your custom logic here
    if (PlayerController)
    {
        PlayerController->UI_Use_Inventory_Item(InteractSlotIndex);
    }
}

void UInteractSlotWidget::OnDropButtonClicked()
{
    // Handle Drop button click
    UE_LOG(LogTemp, Log, TEXT("Drop Button Clicked!"));
    // Add your custom logic here
}