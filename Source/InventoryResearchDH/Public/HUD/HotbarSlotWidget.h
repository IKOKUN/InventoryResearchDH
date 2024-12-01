// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/ItemInformation.h"
#include "HotbarSlotWidget.generated.h"

class UTexture2D;
class UDragItem;
class UToolTipWidget;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UHotbarSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHotbarSlotWidget(const FObjectInitializer& ObjectInitializer);

	TObjectPtr<UToolTipWidget> SlotToolTipInfo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FItemInformation HotbarItemInformation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	int32 HotBarNumber = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	TObjectPtr<UTexture2D> BackgroundIcon;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FSlateBrush GetIconBrush() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetNumberText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	UToolTipWidget* GetToolTipWidget() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag Widget")
	TSubclassOf<UDragItem> DragItemClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag Widget")
	TSubclassOf<UUserWidget> DraggedItemWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag Widget")
	TSubclassOf<UUserWidget> ToolTipWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FEventReply  IconOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
