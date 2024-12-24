// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphicSettingWidget.generated.h"

class UComboBoxString;
class APlayerController;
class UButton;
class UGameUserSettings;
class USlider;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class VIDEOGAMESETTING_API UGraphicSettingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    // Fungsi untuk menangani input Tab
    UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
    void ToggleWidgetVisibility();

    // Fungsi untuk mendapatkan setting saat ini
	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void GetCurrentSettings();

    FString SelectingConventer(int32 LevelBase);
	int32 SelectionConventer(FString Selection);
    FString SelectionConventerWindowMode(EWindowMode::Type WindowMode);

	// Bind Setiap Combo Box
	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnQualitySettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnWindowModeSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnResolutionSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnTextureSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnViewDistanceSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnAntiAliasingSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget") 
	void OnPostProcessingSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnReflectionSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnShadowSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
    void OnGlobalIlluminationSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnParticleEffectSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnFoliageSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnShadingSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnVSyncSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
    void OnColorBlindSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
    void OnColorBlindStrengthSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "GraphicSettingWidget")
	void OnRecommendedSettingsButtonClicked();


	// Fungsi Untuk Mengubah Brightness

     /** Callback for when the slider value changes */
    UFUNCTION()
    void OnBrightnessSliderValueChanged(float Value);

    /** Callback for when the text in the text box changes */
    UFUNCTION()
    void OnGammaTextChanged(const FText& Text);

    /** Callback for when the text box loses focus or text is committed */
    UFUNCTION()
    void OnGammaTextCommitted(const FText& Text, ETextCommit::Type CommitType);

    /** Updates the gamma value in the engine settings */
    void UpdateGammaFromSlider(float SliderValue);

    /** Updates the gamma value from text input */
    void UpdateGammaFromText(const FString& TextValue);

    /** Callback for when the slider value changes */
    UFUNCTION()
    void OnResolutionQualitySliderValueChanged(float Value);

    /** Callback for when the text in the text box changes */
    UFUNCTION()
    void OnResolutionQualityTextChanged(const FText& Text);

    /** Callback for when the text box loses focus or text is committed */
    UFUNCTION()
    void OnResolutionQualityTextCommitted(const FText& Text, ETextCommit::Type CommitType);

    /** Updates the gamma value in the engine settings */
    void UpdateResolutionQualityFromSlider(float SliderValue);

    /** Updates the gamma value from text input */
    void UpdateResolutionQualityFromText(const FString& TextValue);


	void SetResolutionQuality(float ResolutionQualityEditor);
    void SetVSyncEditor(bool bEnableVSyncEditor);

protected:
    /** Slider for adjusting gamma */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> ResolutionQualitySlider;

    /** Text block to display the gamma value */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> ResolutionQualityTextBox;

	// For Setting Brightness
	// Combo Box for Color Blind
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ColorBlindSetting;

	// Combo Box for Color Blind Strength
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ColorBlindStrengthSetting;

    /** Slider for adjusting gamma */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> GammaSlider;

    /** Text block to display the gamma value */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> GammaValueTextBox;

	// Recomended Settings Button
	UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> RecommendedSettingsButton;

    // Referensi ke combo box dari blueprint
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* QualitySetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* WindowModeSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* TextureSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ViewDistanceSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* AntiAliasingSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* PostProcessingSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ReflectionSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ShadowSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* GlobalIlluminationSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ParticleEffectSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* FoliageSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ShadingSetting;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* VSyncSetting;

private:
    // Menyimpan referensi ke PlayerController
    TObjectPtr<APlayerController> PlayerController;
	bool bFirstInitialization = false;
	UGameUserSettings* GameUserSettings;


    EColorVisionDeficiency SelectingColorBlindMode = EColorVisionDeficiency::NormalVision;
	int32 SelectingColorBlindStrength = 10;

};
