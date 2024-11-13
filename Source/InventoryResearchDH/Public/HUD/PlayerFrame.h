// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerFrame.generated.h"

class AIRPlayerController;
class UImage;
class UProgressBar;
class UDragWidget;
class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UPlayerFrame : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerFrame(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	TObjectPtr<AIRPlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	FVector2D DragWindowOffset;

	/* Bind Widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> Health;
	/* End Bind Widget */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag Widget")
	TSubclassOf<UDragWidget> DragWidgetClass;

	// Fungsi untuk mendapatkan nama karakter
	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetNameText() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetHealthText() const;

	// Fungsi untuk mendapatkan persentase kesehatan
	UFUNCTION(BlueprintCallable, Category = "Binding Function")
	float GetHealthPercent() const;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
