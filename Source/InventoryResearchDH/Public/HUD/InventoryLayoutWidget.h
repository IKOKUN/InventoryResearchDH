// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryLayoutWidget.generated.h"

class UInventorySlotWidget;
class UCanvasPanel;
class UContainerWidget;
class UEquipmentWidget;
class UInventoryWidget;
class UHotbarWidget;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UInventoryLayoutWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInventoryLayoutWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	TArray<UInventorySlotWidget*> InventorySlotWidgets;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FVector2D DragWindowOffset;

	/* Bind Widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UContainerWidget> ContainerWidgetComp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipmentWidget> EquipmentWidgetComp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryWidget> InventoryWidgetComp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHotbarWidget> HotbarWidgetComp;
	/* End Bind Widget */

	TObjectPtr<AIRPlayerController> PlayerController;
protected:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
