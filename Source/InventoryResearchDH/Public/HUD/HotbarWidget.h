// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HotbarWidget.generated.h"

class UHotbarSlotWidget;
class UDragWidget;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UHotbarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHotbarWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hotbar")
	TArray<class UHotbarSlotWidget*> HotbarSlots;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hotbar")
	bool bIsHotbarLocked = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag Widget")
    TSubclassOf<UDragWidget> DragWidgetClass;

    // Bind setiap slot widget ke kode C++
    UPROPERTY(meta = (BindWidget))
    UHotbarSlotWidget* HotbarSlot_0;

    UPROPERTY(meta = (BindWidget))
    UHotbarSlotWidget* HotbarSlot_1;

    UPROPERTY(meta = (BindWidget))
    UHotbarSlotWidget* HotbarSlot_2;

    UPROPERTY(meta = (BindWidget))
    UHotbarSlotWidget* HotbarSlot_3;

    UPROPERTY(meta = (BindWidget))
    UHotbarSlotWidget* HotbarSlot_4;

    UPROPERTY(meta = (BindWidget))
    UHotbarSlotWidget* HotbarSlot_5;

    UPROPERTY(meta = (BindWidget))
    UHotbarSlotWidget* HotbarSlot_6;

    UPROPERTY(meta = (BindWidget))
    UHotbarSlotWidget* HotbarSlot_7;
protected:
	virtual void NativeConstruct() override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
