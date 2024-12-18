// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/InventoryItem.h"
#include "InventoryWidget.generated.h"

class UInventorySlotWidget;
class AIRPlayerController;
class UButton;
class UTextBlock;
class UUniformGridPanel;
class UDragWidget;
class UComboBoxString;
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

	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	AIRPlayerController* PlayerController;

	/* Bind Widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Title;
	/* End Bind Widget */

	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	FText GetGoldText() const;

	/* Bind Function */
	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	ESlateVisibility GetInventoryVisibility() const;

	/* Sorting Widget*/
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> SortingInventoryComboBox;

	TMap<FString, TArray<FInventoryItem>> CachedSortingInventoryMap;

	// Tambahkan variabel untuk melacak status sorting saat ini
	FString CurrentSortingStatus = "Sort By"; // Default adalah "Sort By"

	UFUNCTION()
	void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	void SetSortingInventoryMapFromRarerity(const FString& SortingKey, bool bDescending, int32 SlotThreshold = 14);
	void SetSortingInventoryMapFromNewest();
	void SetSortingInventoryMapFromOldest();
	/* End Sorting Widget*/
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	//virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
