// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/ItemInformation.h"
#include "InteractSlotWidget.generated.h"

class UButton;
class AIRPlayerController;
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

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	FItemInformation InteractSlotItemInformation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	int32 InteractSlotIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Controller")
	TObjectPtr<AIRPlayerController> PlayerController;
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnEquipButtonClicked();

	UFUNCTION()
	void OnDropButtonClicked();
};
