// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Container.generated.h"

class AIRPlayerController;
class UContainerSlot;
class UButton;
class USizeBox;
class UScrollBox;
class UGridPanel;
class UDragWidget;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UContainer : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UContainer(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	AIRPlayerController* PlayerController;

	/* Bind Widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> ContainerGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> ContainerSize;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;
	/* End Bind Widget */

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	ESlateVisibility ContainerVisibility = ESlateVisibility::Hidden;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FVector2D DragWindowOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	bool bIsStorageContainer = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FName ContainerName = FName("Container");

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	TArray<UContainerSlot*> ContainerSlots;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	bool bIsWindowLocked = false;

	UFUNCTION(BlueprintCallable, Category = "Bind Function")
	FText GetTitleText() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drag Widget")
	TSubclassOf<UDragWidget> DragWidgetClass;
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

};
