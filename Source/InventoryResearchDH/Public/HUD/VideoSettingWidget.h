// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VideoSettingWidget.generated.h"

class USlider;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UVideoSettingWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    /** Callback for when the slider value changes */
    UFUNCTION()
    void OnBrightnessSliderValueChanged(float Value);

    /** Callback for when the text in the text box changes */
    UFUNCTION()
    void OnGammaTextChanged(const FText& Text);

    /** Callback for when the text box loses focus or text is committed */
    UFUNCTION()
    void OnGammaTextCommitted(const FText& Text, ETextCommit::Type CommitType);

private:
    /** Slider for adjusting gamma */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> GammaSlider;

    /** Text block to display the gamma value */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> GammaValueTextBox;

    /** Updates the gamma value in the engine settings */
    void UpdateGammaFromSlider(float SliderValue);

    /** Updates the gamma value from text input */
    void UpdateGammaFromText(const FString& TextValue);
};
