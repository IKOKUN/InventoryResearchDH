// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/ItemInformation.h"
#include "../Data/EquipmentSlots.h"
#include "ContainerSlotWidget.generated.h"

class AIRPlayerController;
class UTexture2D;
class UBorder;
class UToolTipWidget;
class UDragItem;
class UDraggedItemWidget;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UContainerSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UContainerSlotWidget(const FObjectInitializer& ObjectInitializer);

	TObjectPtr<UToolTipWidget> SlotToolTipInfo;

	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	AIRPlayerController* PlayerController;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	int32 ContainerSlot;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	TObjectPtr<UTexture2D> BackgroundIcon;

	// Bind Widget
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	/* Bind Event To Get Variable In Blueprint*/
	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetBorderVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetAmountText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FLinearColor GetBorderColor() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetNameVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetNameText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Function")
	UToolTipWidget* GetToolTipWidget() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FSlateBrush GetIconBrush() const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FItemInformation ItemInformation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	bool bIsStorageSlot = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	bool bIsSlotHovered = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	bool bIsRightMouseButtonDown = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	TSubclassOf<UUserWidget> ToolTipWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	TSubclassOf<UDragItem> DragItemClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	TSubclassOf<UDraggedItemWidget> DraggedItemWidgetClass;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// Fungsi untuk mendapatkan jumlah entri dalam EEquipmentSlots
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helper")
	FORCEINLINE int32 GetNumberOfEquipmentSlots() const {
		return static_cast<int32>(EEquipmentSlots::EEquipmentSlots_MAX);
	}
};
