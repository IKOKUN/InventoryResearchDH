// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Drawing/DrawingWidget.h"

#include "HUD/Drawing/DotDrawWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/UserInterfaceSettings.h"



void UDrawingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Spawn 5 dots
	SpawnRandomDots(DotCount);
}

void UDrawingWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    LinePoints.Empty();
    TemporaryLinePoints.Empty();
    bIsDrawing = false;
    LastDotIndex = -1;
}

FReply UDrawingWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    //FVector2D ClickPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
    //FVector2D ClickPosition = InGeometry.AbsoluteToLocal(InMouseEvent->GetCachedGeometry().GetScreenSpacePosition().GetAbsolutePosition());
    FVector2D ClickPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
    UE_LOG(LogTemp, Warning, TEXT("Click Position: (%f, %f)"), ClickPosition.X, ClickPosition.Y);

    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        UDotDrawWidget* DotWidget = DotWidgets[i];
        //FVector2D DotPosition = DotWidget->GetCachedGeometry().GetAbsolutePosition();
        FVector2D DotPosition = InGeometry.AbsoluteToLocal(DotWidget->GetCachedGeometry().GetAbsolutePosition());

        UE_LOG(LogTemp, Warning, TEXT("Dot %d Position: (%f, %f)"), i, DotPosition.X, DotPosition.Y);

        // Menghitung batas-batas kotak Dot (misalnya ukuran Dot 50x50)
        float DotLeft = DotPosition.X - DotSize.X / 2;
        float DotRight = DotPosition.X + DotSize.X / 2;
        float DotTop = DotPosition.Y - DotSize.Y / 2;
        float DotBottom = DotPosition.Y + DotSize.Y / 2;

        UE_LOG(LogTemp, Warning, TEXT("Dot %d Position: (%f, %f), Left: %f, Right: %f, Top: %f, Bottom: %f"),
            i, DotPosition.X, DotPosition.Y, DotLeft, DotRight, DotTop, DotBottom);

        // Memeriksa apakah posisi klik berada dalam area Dot
        if (ClickPosition.X >= DotLeft && ClickPosition.X <= DotRight &&
            ClickPosition.Y >= DotTop && ClickPosition.Y <= DotBottom)
        {
            UE_LOG(LogTemp, Warning, TEXT("Dot %d clicked!"), i);

            if (!bIsDrawing)
            {
                bIsDrawing = true;
                LastDotIndex = i;
                UE_LOG(LogTemp, Warning, TEXT("Drawing started from Dot %d."), i);
            }
            else if (LastDotIndex != i)
            {
                FVector2D LastDotPosition = DotWidgets[LastDotIndex]->GetCachedGeometry().GetAbsolutePosition();
                TemporaryLinePoints.Empty();
                LinePoints.Add(LastDotPosition);
                LinePoints.Add(DotPosition);
                LastDotIndex = i;

                UE_LOG(LogTemp, Warning, TEXT("Line drawn from Dot %d to Dot %d."), LastDotIndex, i);
            }

            return FReply::Handled();
        }
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UDrawingWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    bIsDrawing = false;
    TemporaryLinePoints.Empty();
    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UDrawingWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (!bIsDrawing) return Super::NativeOnMouseMove(InGeometry, InMouseEvent);

    FVector2D CursorPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    TemporaryLinePoints.Empty();
    if (LastDotIndex >= 0 && LastDotIndex < DotWidgets.Num())
    {
        UDotDrawWidget* LastDot = DotWidgets[LastDotIndex];
        FVector2D LastDotPosition = LastDot->GetCachedGeometry().GetAbsolutePosition();
        TemporaryLinePoints.Add(LastDotPosition);
        TemporaryLinePoints.Add(CursorPosition);
    }

    return FReply::Handled();
}

void UDrawingWidget::SpawnRandomDots(int32 Count)
{
    if (!DotDrawWidgetClass || !DrawCanvasPanel || !CanvasBorder) return;

    // Get CanvasBorder size
    FVector2D BorderSize = FVector2D(640.f, 640.f); // Assuming fixed size, adjust if dynamic
    DotSize = FVector2D(50.0f, 50.0f); // Size of DotDrawWidget

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
            if (i == 0)
            {
                // Set focus to the first dot
                TSharedRef<SWidget> DotWidgetRef = DotWidget->TakeWidget();
                FSlateApplication::Get().SetUserFocus(0, DotWidgetRef, EFocusCause::SetDirectly);
            }
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
    int32 CurrentLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    if (DotWidgets.Num() < 2) return CurrentLayer;

    FLinearColor LineColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);
    float LineThickness = 2.0f;

    for (int32 i = 0; i < DotWidgets.Num() - 1; i++)
    {
        const UDotDrawWidget* DotA = DotWidgets[i];
        const UDotDrawWidget* DotB = DotWidgets[i + 1];

        if (!DotA || !DotB) continue;

        const UImage* DotImageA = DotA->GetDotImage();
        const UImage* DotImageB = DotB->GetDotImage();

        if (!DotImageA || !DotImageB) continue;

        FVector2D AbsolutePositionA = AllottedGeometry.AbsoluteToLocal(DotImageA->GetCachedGeometry().GetAbsolutePosition());
        FVector2D AbsolutePositionB = AllottedGeometry.AbsoluteToLocal(DotImageB->GetCachedGeometry().GetAbsolutePosition());

        FVector2D Offset(30.0f, 30.0f);
        AbsolutePositionA += Offset;
        AbsolutePositionB += Offset;

        // Ubah nama variabel lokal
        TArray<FVector2D> LineSegmentPoints = { AbsolutePositionA, AbsolutePositionB };

        FSlateDrawElement::MakeLines(
            OutDrawElements,
            CurrentLayer,
            AllottedGeometry.ToPaintGeometry(),
            LineSegmentPoints,
            ESlateDrawEffect::None,
            LineColor,
            true,
            LineThickness
        );
    }

    // Draw temporary line
    if (TemporaryLinePoints.Num() == 2)
    {
        FSlateDrawElement::MakeLines(
            OutDrawElements,
            CurrentLayer + 1,
            AllottedGeometry.ToPaintGeometry(),
            TemporaryLinePoints,
            ESlateDrawEffect::None,
            FLinearColor::Black, // Black line
            true,
            2.0f
        );
    }

    return CurrentLayer;
}


/*
int32 UDrawingWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int32 CurrentLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    if (DotWidgets.Num() < 2) return CurrentLayer;

    FLinearColor LineColor = FLinearColor::Black;
    float LineThickness = 2.0f;

    // Logging resolusi viewport dan DPI
    FVector2D ViewportSize = AllottedGeometry.GetLocalSize();
    float DPI = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
    UE_LOG(LogTemp, Warning, TEXT("Viewport Size: X=%f, Y=%f, DPI=%f"), ViewportSize.X, ViewportSize.Y, DPI);

    for (int32 i = 0; i < DotWidgets.Num() - 1; i++)
    {
        const UDotDrawWidget* DotA = DotWidgets[i];
        const UDotDrawWidget* DotB = DotWidgets[i + 1];

        if (!DotA || !DotB) continue;

        // Ambil posisi DotImage di dalam DotDrawWidget
        const UImage* DotImageA = DotA->GetDotImage();
        const UImage* DotImageB = DotB->GetDotImage();

        if (!DotImageA || !DotImageB) continue;

        FVector2D ScreenSize = FVector2D(GSystemResolution.ResX, GSystemResolution.ResY); // Mendapatkan ukuran layar
        UE_LOG(LogTemp, Warning, TEXT("ScreenSize: X=%f, Y=%f"), ScreenSize.X, ScreenSize.Y);

        // Hitung posisi absolut DotImage menggunakan GetCachedGeometry
        // Sesudah
        FVector2D AbsolutePositionA = AllottedGeometry.AbsoluteToLocal(DotImageA->GetCachedGeometry().GetAbsolutePosition());
        FVector2D AbsolutePositionB = AllottedGeometry.AbsoluteToLocal(DotImageB->GetCachedGeometry().GetAbsolutePosition());

        // Tambahkan offset
        FVector2D Offset(35.0f, 40.0f);
        AbsolutePositionA += Offset;
        AbsolutePositionB += Offset;

        // Log posisi absolut untuk debugging
        UE_LOG(LogTemp, Warning, TEXT("AbsolutePositionA: X=%f, Y=%f"), AbsolutePositionA.X, AbsolutePositionA.Y);
        UE_LOG(LogTemp, Warning, TEXT("AbsolutePositionB: X=%f, Y=%f"), AbsolutePositionB.X, AbsolutePositionB.Y);


        // Sesuaikan dengan DPI
        AbsolutePositionA *= DPI;
        AbsolutePositionB *= DPI;

        // Log setelah koreksi DPI
        UE_LOG(LogTemp, Warning, TEXT("AbsolutePositionA (with DPI): X=%f, Y=%f"), AbsolutePositionA.X, AbsolutePositionA.Y);
        UE_LOG(LogTemp, Warning, TEXT("AbsolutePositionB (with DPI): X=%f, Y=%f"), AbsolutePositionB.X, AbsolutePositionB.Y);

        // Gambar garis antara dua titik
        TArray<FVector2D> LinePoints = { AbsolutePositionA, AbsolutePositionB };

        FSlateDrawElement::MakeLines(
            OutDrawElements,
            CurrentLayer,
            AllottedGeometry.ToPaintGeometry(),
            LinePoints,
            ESlateDrawEffect::None,
            LineColor,
            true,
            LineThickness
        );
    }

    return CurrentLayer;
}
*/