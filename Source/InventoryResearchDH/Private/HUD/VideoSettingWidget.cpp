// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/VideoSettingWidget.h"

#include "Components/Slider.h"
#include "Components/EditableTextBox.h"
#include "Engine/Engine.h"


void UVideoSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

    // Bind the slider's value change event
    if (GammaSlider)
    {
        GammaSlider->OnValueChanged.AddDynamic(this, &UVideoSettingWidget::OnBrightnessSliderValueChanged);
        GammaSlider->SetValue(50.0f); // Set default slider value (middle point for example)
    }

    // Bind the text box's text change and commit events
    if (GammaValueTextBox)
    {
        GammaValueTextBox->OnTextChanged.AddDynamic(this, &UVideoSettingWidget::OnGammaTextChanged);
        GammaValueTextBox->OnTextCommitted.AddDynamic(this, &UVideoSettingWidget::OnGammaTextCommitted);

        // Set default text value
        GammaValueTextBox->SetText(FText::FromString(TEXT("50")));
    }

    // Apply initial gamma value based on default slider position
    UpdateGammaFromSlider(50.0f);
}

void UVideoSettingWidget::OnBrightnessSliderValueChanged(float Value)
{
    UpdateGammaFromSlider(Value);
}

void UVideoSettingWidget::UpdateGammaFromSlider(float SliderValue)
{
    // Convert slider value (0-100) to gamma (0.0-4.4)
    float Gamma = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 100.0f), FVector2D(0.0f, 4.4f), SliderValue);

    // Update the gamma in the engine
    if (GEngine)
    {
		GEngine->DisplayGamma = Gamma;
    }

    // Update the text box with the new gamma value
    if (GammaValueTextBox)
    {
        GammaValueTextBox->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), SliderValue)));
    }
}

void UVideoSettingWidget::OnGammaTextChanged(const FText& Text)
{
    FString TextValue = Text.ToString();
    UpdateGammaFromText(TextValue);
}

void UVideoSettingWidget::OnGammaTextCommitted(const FText& Text, ETextCommit::Type CommitType)
{
    FString TextValue = Text.ToString();
    UpdateGammaFromText(TextValue);
}

void UVideoSettingWidget::UpdateGammaFromText(const FString& TextValue)
{
    // Parse text value to float
    float SliderGamma = FCString::Atof(*TextValue);

    // Clamp gamma value to valid range (0.0 to 4.4)
    
    float Gamma = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 100.0f), FVector2D(0.0f, 4.4f), SliderGamma);

    Gamma = FMath::Clamp(Gamma, 0.0f, 4.4f);
    // Update the gamma in the engine
    if (GEngine)
    {
		GEngine->DisplayGamma = Gamma;
    }

    // Update the slider's value to match the gamma
    if (GammaSlider)
    {
        // float SliderValue = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 4.4f), FVector2D(0.0f, 100.0f), SliderGamma);
        GammaSlider->SetValue(SliderGamma);
    }

    // Update the text box with the clamped gamma value (to ensure correctness)
    if (GammaValueTextBox)
    {
        GammaValueTextBox->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), SliderGamma)));
    }
}

