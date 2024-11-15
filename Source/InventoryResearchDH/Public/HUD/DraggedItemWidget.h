// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/ItemInformation.h"
#include "DraggedItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UDraggedItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDraggedItemWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	FItemInformation DraggedItemInformation;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	int32 Amount = 0;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FLinearColor GetBorderColor() const;

	UFUNCTION(BlueprintCallable, Category = "Binding Funtion")
	FText GetAmountText() const;

protected:
	virtual void NativeConstruct() override;
};
