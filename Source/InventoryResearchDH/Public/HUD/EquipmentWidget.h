// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentWidget.generated.h"

class AIRPlayerController;
class UInventorySlotWidget;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UEquipmentWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	bool bIsWindowLocked = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	ESlateVisibility EquipmentVisibility = ESlateVisibility::Hidden;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FVector2D DragWindowOffset;

	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	AIRPlayerController* PlayerController;

	/* Bind Widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ArmorText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DexterityText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> IntelligenceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StrengthText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_5;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_6;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_7;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_8;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_9;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_10;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_11;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_12;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlotWidget> InventorySlot_13;
	/* End Bind Widget */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag Widget")
	TSubclassOf<UDragWidget> DragWidgetClass;

	/* Bind Function */
	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	void OnClicked_CloseButton();

	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	FText GetDamageText() const;

	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	FText GetArmorText() const;

	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	FText GetDexterityText() const;

	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	FText GetIntelligenceText() const;

	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	FText GetStrengthText() const;

	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	ESlateVisibility GetEquipmentVisibility() const;
	/* End Bind Function */

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
