// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphicSettingWidget.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

void UGraphicSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Ambil referensi ke PlayerController
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	GetCurrentSettings();

	// Bind event untuk setiap combo box
	QualitySetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnQualitySettingSelectionChanged);
	WindowModeSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnWindowModeSettingSelectionChanged);
	ResolutionSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnResolutionSettingSelectionChanged);
	TextureSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnTextureSettingSelectionChanged);
	ViewDistanceSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnViewDistanceSettingSelectionChanged);
	AntiAliasingSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnAntiAliasingSettingSelectionChanged);
	PostProcessingSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnPostProcessingSettingSelectionChanged);
	ReflectionSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnReflectionSettingSelectionChanged);
	ShadowSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnShadowSettingSelectionChanged);
	GlobalIlluminationSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnGlobalIlluminationSelectionChanged);
	ParticleEffectSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnParticleEffectSettingSelectionChanged);
	FoliageSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnFoliageSettingSelectionChanged);
	ShadingSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnShadingSettingSelectionChanged);


	// Diubah Sementara menjadi VSyncEditor
	SetVSyncEditor(false);
	VSyncSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnVSyncSettingSelectionChanged);

	// Set Resolution Quality awal ke 100
	SetResolutionQuality(100);
	if (ResolutionQualitySlider)
	{
		ResolutionQualitySlider->OnValueChanged.AddDynamic(this, &UGraphicSettingWidget::OnResolutionQualitySliderValueChanged);
	}

	if (ResolutionQualityTextBox)
	{
		ResolutionQualityTextBox->OnTextChanged.AddDynamic(this, &UGraphicSettingWidget::OnResolutionQualityTextChanged);
		ResolutionQualityTextBox->OnTextCommitted.AddDynamic(this, &UGraphicSettingWidget::OnResolutionQualityTextCommitted);
	}


	// Bind the slider's value change event
	if (GammaSlider)
	{
		GammaSlider->OnValueChanged.AddDynamic(this, &UGraphicSettingWidget::OnBrightnessSliderValueChanged);
	}

	// Bind the text box's text change and commit events
	if (GammaValueTextBox)
	{
		GammaValueTextBox->OnTextChanged.AddDynamic(this, &UGraphicSettingWidget::OnGammaTextChanged);
		GammaValueTextBox->OnTextCommitted.AddDynamic(this, &UGraphicSettingWidget::OnGammaTextCommitted);
	}

	if (ColorBlindSetting)
	{
		ColorBlindSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnColorBlindSelectionChanged);
	}

	// Set Color Blind Strength
	SelectingColorBlindStrength = 10;

	if (ColorBlindStrengthSetting)
	{
		ColorBlindStrengthSetting->OnSelectionChanged.AddDynamic(this, &UGraphicSettingWidget::OnColorBlindStrengthSelectionChanged);
		ColorBlindStrengthSetting->SetSelectedIndex(SelectingColorBlindStrength);
	}
	// Bind event untuk tombol Recommended Settings
	if (RecommendedSettingsButton)
	{
		RecommendedSettingsButton->OnClicked.AddDynamic(this, &UGraphicSettingWidget::OnRecommendedSettingsButtonClicked);
	}
}

void UGraphicSettingWidget::ToggleWidgetVisibility()
{
    UE_LOG(LogTemp, Warning, TEXT("ToggleWidgetVisibility Called"));

    if (IsInViewport())
    {
        UE_LOG(LogTemp, Warning, TEXT("Hiding Widget"));
        RemoveFromParent();

        if (PlayerController)
        {
            FInputModeGameOnly InputMode;
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = false;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Showing Widget"));
        AddToViewport();

        if (PlayerController)
        {
            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = true;
        }
    }
}

void UGraphicSettingWidget::GetCurrentSettings()
{
	// Ambil referensi ke GameUserSettings
	GameUserSettings = GameUserSettings == nullptr ? UGameUserSettings::GetGameUserSettings() : GameUserSettings;

	// Ambil setting saat ini
    FString Quality = SelectingConventer(GameUserSettings->GetOverallScalabilityLevel());
    FString WindowMode = SelectionConventerWindowMode(GameUserSettings->GetFullscreenMode());
    FString Resolution = FString::Printf(TEXT("%d x %d"), GameUserSettings->GetScreenResolution().X, GameUserSettings->GetScreenResolution().Y);
    FString Texture = SelectingConventer(GameUserSettings->GetTextureQuality());
    FString ViewDistance = SelectingConventer(GameUserSettings->GetViewDistanceQuality());
    FString AntiAliasing = SelectingConventer(GameUserSettings->GetAntiAliasingQuality());
    FString PostProcessing = SelectingConventer(GameUserSettings->GetPostProcessingQuality());
    FString Reflection = SelectingConventer(GameUserSettings->GetReflectionQuality());
    FString Shadow = SelectingConventer(GameUserSettings->GetShadowQuality());
	FString GlobalIllumination = SelectingConventer(GameUserSettings->GetGlobalIlluminationQuality());
    FString ParticleEffect = SelectingConventer(GameUserSettings->GetVisualEffectQuality());
    FString Foliage = SelectingConventer(GameUserSettings->GetFoliageQuality());
    FString Shading = SelectingConventer(GameUserSettings->GetShadingQuality());
    // FString VSync = GameUserSettings->IsVSyncEnabled() ? "Enabled" : "Disabled";
	float GammaSliderValue = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 4.4f), FVector2D(0.0f, 100.0f), (GEngine->DisplayGamma));


	// Set setting saat ini ke combo box
    QualitySetting->SetSelectedOption(Quality);
	WindowModeSetting->SetSelectedOption(WindowMode);
	ResolutionSetting->SetSelectedOption(Resolution);
	TextureSetting->SetSelectedOption(Texture);
	ViewDistanceSetting->SetSelectedOption(ViewDistance);
	AntiAliasingSetting->SetSelectedOption(AntiAliasing);
	PostProcessingSetting->SetSelectedOption(PostProcessing);
	ReflectionSetting->SetSelectedOption(Reflection);
	ShadowSetting->SetSelectedOption(Shadow);
	GlobalIlluminationSetting->SetSelectedOption(GlobalIllumination);
	ParticleEffectSetting->SetSelectedOption(ParticleEffect);
	FoliageSetting->SetSelectedOption(Foliage);
	ShadingSetting->SetSelectedOption(Shading);
	VSyncSetting->SetSelectedOption("Disabled");
	GammaSlider->SetValue(GammaSliderValue);
    GammaValueTextBox->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::RoundToInt(GammaSliderValue))));
	ResolutionQualityTextBox->SetText(FText::FromString(FString::Printf(TEXT("%d"), 100)));
}

FString UGraphicSettingWidget::SelectingConventer(int32 LevelBase)
{
    switch (LevelBase)
    {
	case 0:
		return "Low";
		break;
	case 1:
		return "Medium";
		break;
	case 2:
		return "High";
		break;
	case 3:
		return "Epic";
		break;
    case 4:
		return "Ultra";
		break;
    default:
        break;
    }

	return FString();
}

int32 UGraphicSettingWidget::SelectionConventer(FString Selection)
{
	if (Selection == "Low")
	{
		return 0;
	}
	else if (Selection == "Medium")
	{
		return 1;
	}
	else if (Selection == "High")
	{
		return 2;
	}
	else if (Selection == "Epic")
	{
		return 3;
	}
	else if (Selection == "Ultra")
	{
		return 4;
	}
	return int32();
}

FString UGraphicSettingWidget::SelectionConventerWindowMode(EWindowMode::Type WindowMode)
{
	switch (WindowMode)
	{
	case EWindowMode::Fullscreen:
		return "Fullscreen";
		break;
	case EWindowMode::WindowedFullscreen:
		return "Windowed Fullscreen";
		break;
	case EWindowMode::Windowed:
		return "Windowed";
		break;
	default:
		break;
	}
	return FString();
}

void UGraphicSettingWidget::OnQualitySettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (QualitySetting)
	{
		GameUserSettings->SetOverallScalabilityLevel(SelectionConventer(SelectedItem));
		GetCurrentSettings();
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnWindowModeSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (WindowModeSetting)
	{
		EWindowMode::Type LocWindowMode = EWindowMode::Fullscreen;
		if (SelectedItem == "Fullscreen")
		{
			LocWindowMode = EWindowMode::Fullscreen;
		}
		else if (SelectedItem == "Windowed Fullscreen")
		{
			LocWindowMode = EWindowMode::WindowedFullscreen;
		}
		else if (SelectedItem == "Windowed")
		{
			LocWindowMode = EWindowMode::Windowed;
		}

		GameUserSettings->SetFullscreenMode(LocWindowMode);
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnResolutionSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (ResolutionSetting)
	{
		FString LocResolution = SelectedItem;
		TArray<FString> ResolutionArray;
		LocResolution.ParseIntoArray(ResolutionArray, TEXT(" x "), true);

		int32 LocWidth = FCString::Atoi(*ResolutionArray[0]);
		int32 LocHeight = FCString::Atoi(*ResolutionArray[1]);

		GameUserSettings->SetScreenResolution(FIntPoint(LocWidth, LocHeight));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnTextureSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (TextureSetting)
	{		
		GameUserSettings->SetTextureQuality(SelectionConventer(SelectedItem));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnViewDistanceSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (ViewDistanceSetting)
	{
		GameUserSettings->SetViewDistanceQuality(SelectionConventer(SelectedItem));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnAntiAliasingSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (AntiAliasingSetting)
	{
		GameUserSettings->SetAntiAliasingQuality(SelectionConventer(SelectedItem));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnPostProcessingSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (PostProcessingSetting)
	{
		GameUserSettings->SetPostProcessingQuality(SelectionConventer(SelectedItem));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnReflectionSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (ReflectionSetting)
	{
		GameUserSettings->SetReflectionQuality(SelectionConventer(SelectedItem));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnShadowSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (ShadowSetting)
	{
		GameUserSettings->SetShadowQuality(SelectionConventer(SelectedItem));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnGlobalIlluminationSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (GlobalIlluminationSetting)
	{
		GameUserSettings->SetGlobalIlluminationQuality(SelectionConventer(SelectedItem));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnParticleEffectSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (ParticleEffectSetting)
	{
		GameUserSettings->SetVisualEffectQuality(SelectionConventer(SelectedItem));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnFoliageSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (FoliageSetting)
	{
		GameUserSettings->SetVisualEffectQuality(SelectionConventer(SelectedItem));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnShadingSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (ShadingSetting)
	{
		GameUserSettings->SetShadingQuality(SelectionConventer(SelectedItem));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnVSyncSettingSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// Diubah Sementara Jadi VSyncEditor
	if (VSyncSetting)
	{
		if (SelectedItem == "Disabled")
		{
			SetVSyncEditor(false);
			//GameUserSettings->SetVSyncEnabled(false);
		}
		else if (SelectedItem == "Enabled")
		{
			SetVSyncEditor(true);
			//GameUserSettings->SetVSyncEnabled(true);
		}
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::OnColorBlindSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (ColorBlindSetting)
	{
		if (SelectedItem == "Off")
		{
			UE_LOG(LogTemp, Warning, TEXT("Normal Vision"));
			SelectingColorBlindMode = EColorVisionDeficiency::NormalVision;
		}
		else if (SelectedItem == "Deuteranope")
		{
			UE_LOG(LogTemp, Warning, TEXT("Deuteranope"));
			SelectingColorBlindMode = EColorVisionDeficiency::Deuteranope;
		}
		else if (SelectedItem == "Protanope")
		{
			UE_LOG(LogTemp, Warning, TEXT("Protanope"));
			SelectingColorBlindMode = EColorVisionDeficiency::Protanope;
		}
		else if (SelectedItem == "Tritanope")
		{
			UE_LOG(LogTemp, Warning, TEXT("Tritanope"));
			SelectingColorBlindMode = EColorVisionDeficiency::Tritanope;
		}

		FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(SelectingColorBlindMode, SelectingColorBlindStrength, true, false);
	}
}

void UGraphicSettingWidget::OnColorBlindStrengthSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (ColorBlindStrengthSetting)
	{
		SelectingColorBlindStrength = FCString::Strtoi(*SelectedItem, nullptr, 10);  // 10 untuk base decimal
		FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(SelectingColorBlindMode, SelectingColorBlindStrength, true, false);
	}
}

void UGraphicSettingWidget::OnRecommendedSettingsButtonClicked()
{
	GameUserSettings->RunHardwareBenchmark(1, 1, 1);
	GameUserSettings->ApplyHardwareBenchmarkResults();
	GetCurrentSettings();
}

void UGraphicSettingWidget::OnBrightnessSliderValueChanged(float Value)
{
	UpdateGammaFromSlider(Value);
}

void UGraphicSettingWidget::UpdateGammaFromSlider(float SliderValue)
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
		GammaValueTextBox->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::RoundToInt(SliderValue))));
	}
}

void UGraphicSettingWidget::OnGammaTextChanged(const FText& Text)
{
	FString TextValue = Text.ToString();
	UpdateGammaFromText(TextValue);
}

void UGraphicSettingWidget::OnGammaTextCommitted(const FText& Text, ETextCommit::Type CommitType)
{
	FString TextValue = Text.ToString();
	UpdateGammaFromText(TextValue);
}

void UGraphicSettingWidget::UpdateGammaFromText(const FString& TextValue)
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
		GammaValueTextBox->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::RoundToInt(SliderGamma))));
	}
}

void UGraphicSettingWidget::OnResolutionQualitySliderValueChanged(float Value)
{
	UpdateResolutionQualityFromSlider(Value);
}

void UGraphicSettingWidget::OnResolutionQualityTextChanged(const FText& Text)
{
	FString TextValue = Text.ToString();
	UpdateResolutionQualityFromText(TextValue);
}

void UGraphicSettingWidget::OnResolutionQualityTextCommitted(const FText& Text, ETextCommit::Type CommitType)
{
	FString TextValue = Text.ToString();
	UpdateResolutionQualityFromText(TextValue);
}

void UGraphicSettingWidget::UpdateResolutionQualityFromSlider(float SliderValue)
{	
	// Update the text box with the new gamma value
	if (ResolutionQualitySlider && ResolutionQualityTextBox)
	{
		//GameUserSettings->SetReflectionQuality(SelectionConventer(FString::Printf(TEXT("%d"), FMath::RoundToInt(SliderValue))));
		ResolutionQualityTextBox->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::RoundToInt(SliderValue))));
		SetResolutionQuality(SliderValue);
		GameUserSettings->ApplySettings(false);	
	}
}

void UGraphicSettingWidget::UpdateResolutionQualityFromText(const FString& TextValue)
{
	if (ResolutionQualitySlider && ResolutionQualityTextBox)
	{
		//GameUserSettings->SetResolutionS
        //ResolutionQualityTextBox->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::RoundToInt(FCString::Atof(*TextValue)))));		
		ResolutionQualitySlider->SetValue(FCString::Atof(*TextValue));
		SetResolutionQuality(FCString::Atof(*TextValue));
		GameUserSettings->ApplySettings(false);
	}
}

void UGraphicSettingWidget::SetResolutionQuality(float ResolutionQualityEditor)
{
	FString Command = FString::Printf(TEXT("sg.ResolutionQuality %d"), FMath::RoundToInt(ResolutionQualityEditor));
	GEngine->Exec(nullptr, *Command);
	UE_LOG(LogTemp, Log, TEXT("Resolution Quality set to %d"), FMath::RoundToInt(ResolutionQualityEditor));
}

void UGraphicSettingWidget::SetVSyncEditor(bool bEnableVSyncEditor)
{
	FString Command = bEnableVSyncEditor ? TEXT("r.VSyncEditor 1") : TEXT("r.VSyncEditor 0");
	GEngine->Exec(nullptr, *Command);
	UE_LOG(LogTemp, Log, TEXT("VSync %s"), bEnableVSyncEditor ? TEXT("enabled") : TEXT("disabled"));
}



