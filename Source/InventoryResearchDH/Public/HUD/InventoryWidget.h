// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventorySlotWidget;
class AIRPlayerController;
class UButton;
class UTextBlock;
class UUniformGridPanel;
class UDragWidget;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	TArray<UInventorySlotWidget*> InventorySlotWidgets;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	bool bIsWindowLocked = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	ESlateVisibility InventoryVisibility = ESlateVisibility::Hidden;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FVector2D DragWindowOffset;

	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	AIRPlayerController* PlayerController;

	/* Bind Widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Title;
	/* End Bind Widget */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag Widget")
	TSubclassOf<UDragWidget> DragWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	FText GetGoldText() const;

	/* Bind Function */
	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	void OnClicked_CloseButton();

	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	ESlateVisibility GetInventoryVisibility() const;

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
