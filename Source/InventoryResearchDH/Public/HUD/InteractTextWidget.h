// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractTextWidget.generated.h"

class UInventoryManagerComponent;
class AIRPlayerController;
class UTextBlock;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UInteractTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInteractTextWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "Component")
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	TObjectPtr<AIRPlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	ESlateVisibility InteractVisibility;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	FText MessageText;

	/* Bind Widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractText;

	UFUNCTION(BlueprintCallable, Category = "Binding Function")
	ESlateVisibility GetInteractVisibility() const;

protected:
	virtual void NativeConstruct() override;
};
