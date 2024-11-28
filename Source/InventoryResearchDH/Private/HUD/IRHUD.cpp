// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/IRHUD.h"

#include "HUD/HUDLayoutWidget.h"
#include "Blueprint/UserWidget.h"
#include "Controller/IRPlayerController.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "UObject/ConstructorHelpers.h"

AIRHUD::AIRHUD()
{
}

void AIRHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCrosshair();
	DrawPlayerStats();

}

void AIRHUD::BeginPlay()
{
	Super::BeginPlay();

    if (MainHUDWidgetClass)
    {
        MainHUDWidgetInstance = CreateWidget<UHUDLayoutWidget>(GetWorld(), MainHUDWidgetClass);
        if (MainHUDWidgetInstance)
        {
            // Menambahkan widget ke viewport
            MainHUDWidgetInstance->AddToViewport();
        }

        // Panggil InitializePlayer di sini jika Anda memiliki referensi ke player controller
        AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetOwningPlayerController());
        if (PlayerController)
        {
            PlayerController->InitializePlayer();
        }

        if (MainHUDWidgetInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("MainHUDWidgetInstance is valid"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("MainHUDWidgetInstance is not valid"));
        }
    }
}

void AIRHUD::DrawCrosshair()
{
    // Menggambar crosshair di tengah layar
    FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
    float CrosshairSize = 10.0f;

    // Menggambar crosshair sebagai garis
    DrawLine(Center.X - CrosshairSize, Center.Y, Center.X + CrosshairSize, Center.Y, FLinearColor::Red);
    DrawLine(Center.X, Center.Y - CrosshairSize, Center.X, Center.Y + CrosshairSize, FLinearColor::Red);
}

void AIRHUD::DrawPlayerStats()
{

}
