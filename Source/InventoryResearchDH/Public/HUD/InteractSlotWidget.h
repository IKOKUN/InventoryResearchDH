// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/ItemInformation.h"
#include "../Data/EquipmentSlots.h"
#include "../Data/ItemType.h"
#include "InteractSlotWidget.generated.h"

class UButton;
class AIRPlayerController;
class UInventorySlotWidget;
class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UInteractSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInteractSlotWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EquipButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DropButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ActionText;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	FItemInformation InteractSlotItemInformation;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	int32 InteractSlotIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	bool bEquipFromInventory = false;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	bool bEquipFromContainer = false;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	FText EquipButtonActionText = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	ESlateVisibility DropButtonVisibility = ESlateVisibility::Visible;


	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	TObjectPtr<UInventorySlotWidget> ParentSlotWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	TObjectPtr<AIRPlayerController> PlayerController;
protected:
	virtual void NativeConstruct() override;


private:
	UFUNCTION()
	void OnEquipButtonClicked();

	UFUNCTION()
	void OnDropButtonClicked();
public:
	void CloseActiveInteractSlotWidget();

	void SetInteractSlotActionText(EItemType Type);
	void SetDropButtonVisibility(bool bVisible);

	// Fungsi untuk mendapatkan jumlah entri dalam EEquipmentSlots
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helper")
	FORCEINLINE int32 GetNumberOfEquipmentSlots() const {
		return static_cast<int32>(EEquipmentSlots::EEquipmentSlots_MAX);
	}
};
