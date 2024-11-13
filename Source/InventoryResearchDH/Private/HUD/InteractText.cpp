// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InteractText.h"
#include "Controller/IRPlayerController.h"
#include "InventoryComponent/InventoryManagerComponent.h"
#include "Components/TextBlock.h"


UInteractText::UInteractText(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		InventoryManagerComponent = PlayerController->GetInventoryManagerComponent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("IRPlayerController is nullptr"));
	}
}

ESlateVisibility UInteractText::GetInteractVisibility() const
{
	if (InventoryManagerComponent)
	{
		if (InventoryManagerComponent->IsContainerOpen())
		{
			return ESlateVisibility::Hidden;
		}
		else
		{
			return ESlateVisibility::Visible;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is nullptr"));
		return ESlateVisibility::Hidden;
	}
}
