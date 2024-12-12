// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InteractSlotWidget.h"

#include "HUD/InventorySlotWidget.h"
#include "Controller/IRPlayerController.h"
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
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    // Log tipe item yang diterima
    UE_LOG(LogTemp, Log, TEXT("SetInteractSlotActionText called with EItemType: %d"), static_cast<int32>(Type));

    EquipButtonActionText = FText::FromString("Equip");
    UE_LOG(LogTemp, Log, TEXT("Default EquipButtonActionText set to: %s"), *EquipButtonActionText.ToString());

    switch (Type)
    {
    case EItemType::Consumable:
        EquipButtonActionText = FText::FromString("Use");
        UE_LOG(LogTemp, Log, TEXT("EItemType is Consumable. EquipButtonActionText set to: %s"), *EquipButtonActionText.ToString());
        break;

    case EItemType::Equipment:
        UE_LOG(LogTemp, Log, TEXT("EItemType is Equipment. Checking InteractSlotIndex and equipment slots."));

        // Log kondisi untuk Equipment
        UE_LOG(LogTemp, Log, TEXT("InteractSlotIndex: %d, Number of Equipment Slots: %d"), InteractSlotIndex, GetNumberOfEquipmentSlots());

        if (InteractSlotIndex > GetNumberOfEquipmentSlots() - 1)
        {
            EquipButtonActionText = FText::FromString("Equip");
            UE_LOG(LogTemp, Log, TEXT("InteractSlotIndex is greater than available slots. EquipButtonActionText set to: %s"), *EquipButtonActionText.ToString());
        }
        else
        {
            EquipButtonActionText = FText::FromString("Un Equip");
            UE_LOG(LogTemp, Log, TEXT("InteractSlotIndex is within available slots. EquipButtonActionText set to: %s"), *EquipButtonActionText.ToString());
        }
        break;

    default:
        UE_LOG(LogTemp, Warning, TEXT("Unhandled EItemType in SetInteractSlotActionText. No action taken."));
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
        ParentSlotWidget->WBP_InteractSlot->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CloseActiveInteractSlotWidget: Failed to cast to UInventorySlotWidget"));
    }

	SetVisibility(ESlateVisibility::Hidden);
}