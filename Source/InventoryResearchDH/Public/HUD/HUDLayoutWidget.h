// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDLayoutWidget.generated.h"

class UInteractTextWidget;
class UInventoryLayoutWidget;
class UPlayerFrameWidget;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UHUDLayoutWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHUDLayoutWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractTextWidget> InteractText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryLayoutWidget> InventoryLayout;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerFrameWidget> PlayerFrame;
};
