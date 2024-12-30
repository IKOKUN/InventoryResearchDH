// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DotDrawWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UDotDrawWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SequenceTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> DotImage;

	void SetSequenceText(int32 Value) const;

public:
	FORCEINLINE UImage* GetDotImage() const { return DotImage.Get(); }
};
