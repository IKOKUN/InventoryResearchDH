// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Drawing/DrawingWidget.h"

#include "HUD/Drawing/DotDrawWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/UserInterfaceSettings.h"



void UDrawingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Spawn 5 dots
	SpawnRandomDots(DotCount);
}

void UDrawingWidget::SpawnRandomDots(int32 Count)
{
    if (!DotDrawWidgetClass || !DrawCanvasPanel || !CanvasBorder) return;

    // Get CanvasBorder size
    FVector2D BorderSize = FVector2D(600.f, 600.f); // Assuming fixed size, adjust if dynamic
    FVector2D DotSize = FVector2D(50.0f, 50.0f); // Size of DotDrawWidget

    TArray<FVector2D> ExistingPositions; // Store existing positions

    for (int32 i = 0; i < Count; i++)
    {
        FVector2D RandomPosition;
        bool bIsValidPosition = false;

        // Try generating valid random position
        for (int32 Attempt = 0; Attempt < 100; Attempt++) // Limit attempts to avoid infinite loop
        {
            float RandomX = FMath::RandRange(-BorderSize.X / 2.0f, BorderSize.X / 2.0f);
            float RandomY = FMath::RandRange(-BorderSize.Y / 2.0f, BorderSize.Y / 2.0f);
            RandomPosition = FVector2D(RandomX, RandomY);

            bIsValidPosition = true;

            // Check against existing positions
            for (const FVector2D& ExistingPosition : ExistingPositions)
            {
                if (FVector2D::DistSquared(RandomPosition, ExistingPosition) < FMath::Square(DotSize.X))
                {
                    bIsValidPosition = false;
                    break;
                }
            }

            if (bIsValidPosition) break;
        }

        // If no valid position found, skip this dot
        if (!bIsValidPosition) continue;

        // Create DotDrawWidget
        UDotDrawWidget* DotWidget = CreateWidget<UDotDrawWidget>(GetWorld(), DotDrawWidgetClass);
        if (DotWidget)
        {
			DotWidget->SetSequenceText(i + 1);
            // Add to CanvasPanel
            UCanvasPanelSlot* CanvasSlot = DrawCanvasPanel->AddChildToCanvas(DotWidget);
            if (CanvasSlot)
            {
                // Set anchor ke tengah canvas
                CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));  // Set anchor ke tengah
                // Set alignment juga ke tengah widget (0.5, 0.5)
                CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));  // Align berdasarkan tengah widget

                // Set posisi dan ukuran widget
                CanvasSlot->SetPosition(RandomPosition);
                CanvasSlot->SetSize(DotSize);

                // Add position to list
                ExistingPositions.Add(RandomPosition);
            }
        }
		DotWidgets.Add(DotWidget);
    }
}

int32 UDrawingWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    // Panggil logika default bawaan
    int32 CurrentLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    // Debug Line dari (0,0) ke (2000, 2000) untuk memverifikasi skala
    FLinearColor DebugLineColor = FLinearColor::Black;
    TArray<FVector2D> DebugLinePoints = { FVector2D(0, 0), FVector2D(2000, 2000) };

    // Menggambar garis untuk memastikan lokasi debug
    FSlateDrawElement::MakeLines(
        OutDrawElements,
        CurrentLayer,
        AllottedGeometry.ToPaintGeometry(),
        DebugLinePoints,
        ESlateDrawEffect::None,
        DebugLineColor,
        true, // Anti-aliasing
        2.0f  // Ketebalan
    );

    if (DotWidgets.Num() < 2) return CurrentLayer;

    FLinearColor LineColor = FLinearColor::Black;
    float LineThickness = 2.0f;

    // Dapatkan ukuran layar untuk menghitung offset ke tengah
    FVector2D ScreenSize = FVector2D(GSystemResolution.ResX, GSystemResolution.ResY); // Mendapatkan ukuran layar
    FVector2D ScreenCenter = ScreenSize * 0.5f; // Titik tengah layar
    FVector2D Offset(100, 30); // Geser 100 di X dan 30 di Y
    ScreenCenter += Offset; // Update ScreenCenter dengan offset
    UE_LOG(LogTemp, Warning, TEXT("ScreenSize: X=%f, Y=%f"), ScreenSize.X, ScreenSize.Y);
    UE_LOG(LogTemp, Warning, TEXT("ScreenCenter with offset: X=%f, Y=%f"), ScreenCenter.X, ScreenCenter.Y);

    // Ambil posisi global berdasarkan geometry
    FVector2D WidgetAbsolutePosition = AllottedGeometry.GetAbsolutePosition();
    float DPI = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y));

    // Log Resolusi dan DPI
    FVector2D ViewportSize = AllottedGeometry.GetLocalSize();  // Ukuran layar dari viewport
    FVector2D WindowSize = FSlateApplication::Get().GetActiveTopLevelWindow()->GetSizeInScreen();  // Resolusi window

    // Log ukuran viewport dan window
    UE_LOG(LogTemp, Warning, TEXT("Viewport Size: X=%f, Y=%f"), ViewportSize.X, ViewportSize.Y);
    UE_LOG(LogTemp, Warning, TEXT("Window Size: X=%f, Y=%f"), WindowSize.X, WindowSize.Y);
    UE_LOG(LogTemp, Warning, TEXT("DPI Scale: %f"), DPI);

    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        const UCanvasPanelSlot* SlotA = Cast<UCanvasPanelSlot>(DotWidgets[i]->Slot);
        if (!SlotA) continue;

        // Dapatkan posisi relatif dari widget
        FVector2D LocalPositionA = SlotA->GetPosition();
        FVector2D WidgetSizeA = SlotA->GetSize();

        // Log posisi lokal dan ukuran widget
        UE_LOG(LogTemp, Warning, TEXT("LocalPositionA: X=%f, Y=%f"), LocalPositionA.X, LocalPositionA.Y);
        UE_LOG(LogTemp, Warning, TEXT("WidgetSizeA: X=%f, Y=%f"), WidgetSizeA.X, WidgetSizeA.Y);

        // Hitung posisi absolut dan tambahkan offset layar
        FVector2D GlobalPositionA = AllottedGeometry.LocalToAbsolute(LocalPositionA);
        GlobalPositionA -= WidgetSizeA * 0.5f; // Koreksi posisi dengan anchor tengah
        GlobalPositionA += ScreenCenter; // Geser ke tengah layar dengan tambahan offset

        // Log posisi absolut setelah perhitungan
        UE_LOG(LogTemp, Warning, TEXT("GlobalPositionA (absolute): X=%f, Y=%f"), GlobalPositionA.X, GlobalPositionA.Y);

        // Pertimbangkan DPI
        GlobalPositionA *= DPI;

        // Log posisi absolut setelah mengaplikasikan DPI
        UE_LOG(LogTemp, Warning, TEXT("GlobalPositionA (with DPI): X=%f, Y=%f"), GlobalPositionA.X, GlobalPositionA.Y);

        for (int32 j = i + 1; j < DotWidgets.Num(); j++)
        {
            const UCanvasPanelSlot* SlotB = Cast<UCanvasPanelSlot>(DotWidgets[j]->Slot);
            if (!SlotB) continue;

            // Dapatkan posisi relatif dan ukuran widget untuk SlotB
            FVector2D LocalPositionB = SlotB->GetPosition();
            FVector2D WidgetSizeB = SlotB->GetSize();

            // Log posisi lokal dan ukuran widget SlotB
            UE_LOG(LogTemp, Warning, TEXT("LocalPositionB: X=%f, Y=%f"), LocalPositionB.X, LocalPositionB.Y);
            UE_LOG(LogTemp, Warning, TEXT("WidgetSizeB: X=%f, Y=%f"), WidgetSizeB.X, WidgetSizeB.Y);

            // Hitung posisi absolut dan tambahkan offset layar
            FVector2D GlobalPositionB = AllottedGeometry.LocalToAbsolute(LocalPositionB);
            GlobalPositionB -= WidgetSizeB * 0.5f; // Koreksi posisi dengan anchor tengah
            GlobalPositionB += ScreenCenter; // Geser ke tengah layar dengan tambahan offset

            // Log posisi absolut setelah perhitungan
            UE_LOG(LogTemp, Warning, TEXT("GlobalPositionB (absolute): X=%f, Y=%f"), GlobalPositionB.X, GlobalPositionB.Y);

            // Pertimbangkan DPI
            GlobalPositionB *= DPI;

            // Log posisi absolut setelah mengaplikasikan DPI
            UE_LOG(LogTemp, Warning, TEXT("GlobalPositionB (with DPI): X=%f, Y=%f"), GlobalPositionB.X, GlobalPositionB.Y);

            // Gambar garis antara dua titik
            TArray<FVector2D> LinePoints = { GlobalPositionA, GlobalPositionB };
            FSlateDrawElement::MakeLines(
                OutDrawElements,
                CurrentLayer,
                AllottedGeometry.ToPaintGeometry(),
                LinePoints,
                ESlateDrawEffect::None,
                LineColor,
                true, // Anti-aliasing
                LineThickness
            );
        }
    }

    return CurrentLayer;
}


/*
int32 UDrawingWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    // Panggil logika default bawaan
    int32 CurrentLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    // Debug Line dari (0,0) ke (2000, 2000) untuk memverifikasi skala
    FLinearColor DebugLineColor = FLinearColor::Black;
    TArray<FVector2D> DebugLinePoints = { FVector2D(0, 0), FVector2D(2000, 2000) };

    // Menggambar garis untuk memastikan lokasi debug
    FSlateDrawElement::MakeLines(
        OutDrawElements,
        CurrentLayer,
        AllottedGeometry.ToPaintGeometry(),
        DebugLinePoints,
        ESlateDrawEffect::None,
        DebugLineColor,
        true, // Anti-aliasing
        2.0f  // Ketebalan
    );

    if (DotWidgets.Num() < 2) return CurrentLayer;

    FLinearColor LineColor = FLinearColor::Black;
    float LineThickness = 2.0f;

    // Dapatkan ukuran layar untuk menghitung offset ke tengah
    FVector2D ScreenSize = FVector2D(GSystemResolution.ResX, GSystemResolution.ResY); // Mendapatkan ukuran layar
    FVector2D ScreenCenter = ScreenSize * 0.5f; // Titik tengah layar

    // Ambil posisi global berdasarkan geometry dan tambahkan offset ke tengah
    FVector2D WidgetAbsolutePosition = AllottedGeometry.GetAbsolutePosition();
    float DPI = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y));

    // Log Resolusi dan DPI
    FVector2D ViewportSize = AllottedGeometry.GetLocalSize();  // Ukuran layar dari viewport
    FVector2D WindowSize = FSlateApplication::Get().GetActiveTopLevelWindow()->GetSizeInScreen();  // Resolusi window

    // Log ukuran viewport dan window, serta DPI
    UE_LOG(LogTemp, Warning, TEXT("Viewport Size: X=%f, Y=%f"), ViewportSize.X, ViewportSize.Y);
    UE_LOG(LogTemp, Warning, TEXT("Window Size: X=%f, Y=%f"), WindowSize.X, WindowSize.Y);
    UE_LOG(LogTemp, Warning, TEXT("DPI Scale: %f"), DPI);

    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        const UCanvasPanelSlot* SlotA = Cast<UCanvasPanelSlot>(DotWidgets[i]->Slot);
        if (!SlotA) continue;

        // Dapatkan posisi relatif dari widget
        FVector2D LocalPositionA = SlotA->GetPosition();
        FVector2D WidgetSizeA = SlotA->GetSize();

        // Hitung posisi absolut dan tambahkan offset layar
        FVector2D GlobalPositionA = AllottedGeometry.LocalToAbsolute(LocalPositionA);
        GlobalPositionA -= WidgetSizeA * 0.5f; // Koreksi posisi dengan anchor tengah
        GlobalPositionA += ScreenCenter; // Geser ke tengah layar

        // Pertimbangkan DPI
        GlobalPositionA *= DPI;

        for (int32 j = i + 1; j < DotWidgets.Num(); j++)
        {
            const UCanvasPanelSlot* SlotB = Cast<UCanvasPanelSlot>(DotWidgets[j]->Slot);
            if (!SlotB) continue;

            // Dapatkan posisi relatif dan ukuran widget untuk SlotB
            FVector2D LocalPositionB = SlotB->GetPosition();
            FVector2D WidgetSizeB = SlotB->GetSize();

            // Hitung posisi absolut dan tambahkan offset layar
            FVector2D GlobalPositionB = AllottedGeometry.LocalToAbsolute(LocalPositionB);
            GlobalPositionB -= WidgetSizeB * 0.5f; // Koreksi posisi dengan anchor tengah
            GlobalPositionB += ScreenCenter; // Geser ke tengah layar

            // Pertimbangkan DPI
            GlobalPositionB *= DPI;

			// GlobalPositionA = GlobalPositionA + 100;
			// GlobalPositionB = GlobalPositionB + 100;

            // Gambar garis antara dua titik
            TArray<FVector2D> LinePoints = { GlobalPositionA, GlobalPositionB };
            FSlateDrawElement::MakeLines(
                OutDrawElements,
                CurrentLayer,
                AllottedGeometry.ToPaintGeometry(),
                LinePoints,
                ESlateDrawEffect::None,
                LineColor,
                true, // Anti-aliasing
                LineThickness
            );
        }
    }

    return CurrentLayer;
}*/