// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ItemInformation.h"
#include "InspectWidget.generated.h"

class UToolTipWidget;
class AIRPlayerController;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UInspectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInspectWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, Category = "Controller")
	TObjectPtr<AIRPlayerController> PlayerController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> ToolTipWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "Properties")
	FItemInformation InvSlotItemInformation;

	/* Bind Widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UToolTipWidget> InspectToolTipWidget;
protected:
	virtual void NativeConstruct() override;
	void SetToolTipWidget();
};
