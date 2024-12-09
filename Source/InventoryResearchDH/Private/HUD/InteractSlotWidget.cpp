// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InteractSlotWidget.h"

#include "HUD/InventorySlotWidget.h"
#include "Controller/IRPlayerController.h"
#include "InventoryComponent/EquipmentInventoryComponent.h"
#include "InventoryComponent/InventoryManagerComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

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
    // UE_LOG(LogTemp, Log, TEXT("Equip Button Clicked! From Index : %d"), InteractSlotIndex);
    // Add your custom logic here
	// Cek apakah item berasal dari inventory
	
    if (PlayerController)
    {
        PlayerController->UI_Use_Inventory_Item(InteractSlotIndex);
    }

    CloseActiveInteractSlotWidget();
}

void UInteractSlotWidget::OnDropButtonClicked()
{
    // Handle Drop button click
    UE_LOG(LogTemp, Log, TEXT("Drop Button Clicked!"));
    if (PlayerController)
    {
        if (InteractSlotIndex > GetNumberOfEquipmentSlots() - 1)
        {
            PlayerController->UI_Drop_Inventory_Item(InteractSlotIndex);
        }
        else
        {
            PlayerController->UI_Use_Inventory_Item(InteractSlotIndex);
        }
        
    }

	CloseActiveInteractSlotWidget();
}

void UInteractSlotWidget::SetInteractSlotActionText(EItemType Type)
{
    // Fix for Problem 1: Remove the invalid assignment
    // EquipButtonActionText = FText::FromString("Equip");

    // Fix for Problem 2: Correct the missing closing quote and invalid variable name
    // EquipButtonActionTextEquip = FText::FromString("Un Equ
    EquipButtonActionText = FText::FromString("Equip");

    // Fix for Problem 3: Correct the switch statement syntax
    switch (Type)
    {
    case EItemType::Consumable:
        EquipButtonActionText = FText::FromString("Use");
        break;
    case EItemType::Equipment:
        if (InteractSlotIndex > GetNumberOfEquipmentSlots() - 1)
        {
            EquipButtonActionText = FText::FromString("Equip");
        }
        else
        {
            EquipButtonActionText = FText::FromString("Un Equip");
        }
        break;
    default:
        break;
    }
}

void UInteractSlotWidget::SetDropButtonVisibility(bool bVisible)
{
    DropButtonVisibility = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
}

void UInteractSlotWidget::CloseActiveInteractSlotWidget()
{
    if (ParentSlotWidget)
    {
        ParentSlotWidget->bIsRightMouseButtonDown = false;
        ParentSlotWidget->WidgetSlotInteract = nullptr;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CloseActiveInteractSlotWidget: Failed to cast to UInventorySlotWidget"));
    }

    RemoveFromParent();
    Destruct();
}