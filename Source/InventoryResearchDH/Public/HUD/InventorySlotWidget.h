// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/ItemInformation.h"
#include "../Data/EquipmentSlots.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UBorder;
class UTexture2D;
class AIRPlayerController;
class UDraggedItemWidget;
class UDragItem;
class UToolTipWidget;
class UButton;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInventorySlotWidget(const FObjectInitializer& ObjectInitializer);

	TObjectPtr<UToolTipWidget> SlotToolTipInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Background;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	int32 InventorySlotIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	TObjectPtr<UTexture2D> BackgroundIcon;

	UPROPERTY(BlueprintReadWrite, Category = "Properties")
	bool bIsRightMouseButtonDown = false;

	UPROPERTY(BlueprintReadWrite, Category = "Properties")
	bool bIsSlotHovered = false;

	UPROPERTY(BlueprintReadWrite, Category = "Properties")
	FItemInformation InvSlotItemInformation;

	UPROPERTY(BlueprintReadWrite, Category = "Controller")
	TObjectPtr<AIRPlayerController> PlayerController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	TSubclassOf<UDragItem> DragItemClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	TSubclassOf<UDraggedItemWidget> DraggedItemWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> ToolTipWidgetClass;

	/* Binding Function */
	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetBorderVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetAmountText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FLinearColor GetBorderColor() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetBackgroundVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	UToolTipWidget* GetToolTipWidget() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FSlateBrush GetIconBrush() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FEventReply  IconOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	/* End Binding Function*/
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:

	// Fungsi untuk mendapatkan jumlah entri dalam EEquipmentSlots
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helper")
	FORCEINLINE int32 GetNumberOfEquipmentSlots() const {
		return static_cast<int32>(EEquipmentSlots::EEquipmentSlots_MAX);
	}
};
