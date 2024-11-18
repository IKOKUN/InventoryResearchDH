// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/ItemInformation.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UBorder;
class UTexture2D;
class AIRPlayerController;
class UDraggedItemWidget;
class UDragItem;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInventorySlotWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Background;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	int32 InventorySlot = 0;

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

	UFUNCTION(BlueprintCallable, Category = "Inv Slot Function")
	static int32 GetNumberOfEquipmentSlots();

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
	UUserWidget* GetToolTipWidget() const;

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
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
