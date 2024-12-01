// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/IRHUD.h"

#include "HUD/HUDLayoutWidget.h"
#include "Blueprint/UserWidget.h"
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
