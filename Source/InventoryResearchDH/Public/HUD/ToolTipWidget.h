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

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	FToolTipInfo ItemToolTipInfo;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTextBlock> ArmorText;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTextBlock> DescriptionText;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTextBlock> DexterityText;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTextBlock> IntelligenceText;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTextBlock> StackSizeText;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTextBlock> StrengthText;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTextBlock> TypeText;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTextBlock> UseText;

	/*Binding Function*/
	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetBorderVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FLinearColor GetBorderColor() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetToolTipVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetNameText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetTypeText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetDamageText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetArmorText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetStackSizeText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetStrengthText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetDexterityText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetIntelligenceText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetUseText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetDescriptionText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetDamageVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetArmorVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetStackSizeVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetStrengthVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetDexterityVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetIntelligenceVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetUseVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	ESlateVisibility GetDescriptionVisibility() const;
	/*End Binding Function*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag Widget")
	TSubclassOf<UDragWidget> DragWidgetClass;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
