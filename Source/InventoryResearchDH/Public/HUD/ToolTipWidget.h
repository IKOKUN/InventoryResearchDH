// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/ToolTipInfo.h"
#include "ToolTipWidget.generated.h"

class UTextBlock;
class UDragWidget;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UToolTipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UToolTipWidget(const FObjectInitializer& ObjectInitializer);

	// Binding Widget
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TypeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ArmorText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StackSizeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StrengthText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DexterityText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> IntelligenceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UseText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText;
	// End Binding Widget

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	FToolTipInfo ItemToolTipInfo;

	/*Binding Function*/
	FLinearColor GetBorderColor() const;

	FText GetNameText() const;
	FText GetTypeText() const;
	FText GetDamageText() const;
	FText GetArmorText() const;
	FText GetStackSizeText() const;
	FText GetStrengthText() const;
	FText GetDexterityText() const;
	FText GetIntelligenceText() const;
	FText GetUseText() const;
	FText GetDescriptionText() const;

	ESlateVisibility GetBorderVisibility() const;
	ESlateVisibility GetDamageVisibility() const;
	ESlateVisibility GetArmorVisibility() const;
	ESlateVisibility GetStackSizeVisibility() const;
	ESlateVisibility GetStrengthVisibility() const;
	ESlateVisibility GetDexterityVisibility() const;
	ESlateVisibility GetIntelligenceVisibility() const;
	ESlateVisibility GetUseVisibility() const;
	ESlateVisibility GetDescriptionVisibility() const;

	void SetToolTipWidget() const;
	/*End Binding Function*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag Widget")
	TSubclassOf<UDragWidget> DragWidgetClass;

protected:
	//virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
