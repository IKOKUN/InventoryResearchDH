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

	UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<UWidgetAnimation> ConnectionDotAnimation;

	void SetSequenceText(int32 Value) const;
	void SetDotImageBrushColorToGreen();
	void SetDotImageTexture(bool bComplete);
	void PlayDotAnimation();
	FVector2D GetDotCenterPosition() const;

	UPROPERTY(EditAnywhere, Category = "Dot Properties")
	bool bIsCurveDot = false;


	UPROPERTY(EditAnywhere, Category = "Dot Properties")
	int32 DotIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Dot Properties")
	UTexture2D* ConnectionDotTexture;

	UPROPERTY(EditAnywhere, Category = "Dot Properties")
	UTexture2D* CompleteDotTexture;

	UPROPERTY(EditAnywhere, Category = "Dot Properties")
	bool bIsRightDirection = true;

	UPROPERTY(EditAnywhere, Category = "Dot Properties")
	bool bIsDoubleArch = false;

    UPROPERTY(EditAnywhere, Category = "Dot Properties")
    float AdditiveDirection1 = 0.33f;

	UPROPERTY(EditAnywhere, Category = "Dot Properties")
	float AdditiveDirection2 = 0.66f;

    UPROPERTY(EditAnywhere, Category = "Dot Properties")
    float CurveOffset = 200.0f;
public:
	FORCEINLINE UImage* GetDotImage() const { return DotImage.Get(); }
};
