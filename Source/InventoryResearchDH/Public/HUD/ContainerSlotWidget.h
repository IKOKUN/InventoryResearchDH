// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/ItemInformation.h"
#include "ContainerSlotWidget.generated.h"

class AIRPlayerController;
class UTexture2D;
class UBorder;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UContainerSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UContainerSlotWidget(const FObjectInitializer& ObjectInitializer);

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
	UUserWidget* GetToolTipWidget() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FSlateBrush GetIconBrush() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FEventReply  IconOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FItemInformation ItemInformation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsStorageSlot = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsSlotHovered = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsRightMouseButtonDown = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> ToolTipWidgetClass;
};
