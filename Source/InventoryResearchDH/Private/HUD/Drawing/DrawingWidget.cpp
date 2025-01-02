// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Drawing/DrawingWidget.h"

#include "HUD/Drawing/DotDrawWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/UserInterfaceSettings.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/GameViewportClient.h" // Required for accessing the viewport client

void UDrawingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Spawn dots sesuai jumlah yang diinginkan
    SpawnRandomDots(DotCount);
}

void UDrawingWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    LinePoints.Empty();
    TemporaryLinePoints.Empty();
    bIsDrawing = false;
    LastDotIndex = 0; // Tidak ada Dot aktif pada awalnya
}

float UDrawingWidget::GetDPIScale()
{
    return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(GetWorld()->GetGameViewport()->Viewport->GetSizeXY());
}

FVector2D UDrawingWidget::GetViewportLocalPosition(FVector2D ScreenPosition)
{
    if (UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport())
    {
        float DPI = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(ViewportClient->Viewport->GetSizeXY());
        return ScreenPosition / DPI;
    }
    return ScreenPosition;
}

FReply UDrawingWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // Posisi klik mouse dalam ruang lokal viewport
    FVector2D ClickPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
    UE_LOG(LogTemp, Warning, TEXT("Mouse Button Down at: %s"), *ClickPosition.ToString());

    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        UDotDrawWidget* DotWidget = DotWidgets[i];
        if (!DotWidget || !DotWidget->DotImage)
        {
            continue;
        }

        // Ambil posisi lokal DotImage terhadap widget utama
        FVector2D DotImageLocalPosition = InGeometry.AbsoluteToLocal(DotWidget->DotImage->GetCachedGeometry().GetAbsolutePosition());
        FVector2D DotImageSize = DotWidget->DotImage->GetCachedGeometry().GetAbsoluteSize();

        // Perhitungan tepi Dot
        FVector2D DotLeftTop = DotImageLocalPosition - (DotImageSize * 0.5f);
        FVector2D DotRightBottom = DotImageLocalPosition + (DotImageSize * 0.5f);

        // Tambahkan margin kecil (opsional)
        float Margin = 5.f;
        DotLeftTop -= FVector2D(Margin, Margin);
        DotRightBottom += FVector2D(Margin, Margin);

        UE_LOG(LogTemp, Warning, TEXT("Checking Dot %d with Bounds: LeftTop=%s, RightBottom=%s"),
            i, *DotLeftTop.ToString(), *DotRightBottom.ToString());

        // Periksa apakah klik berada di dalam area Dot
        if (ClickPosition.X >= DotLeftTop.X && ClickPosition.X <= DotRightBottom.X &&
            ClickPosition.Y >= DotLeftTop.Y && ClickPosition.Y <= DotRightBottom.Y)
        {
            UE_LOG(LogTemp, Warning, TEXT("Mouse clicked inside Dot %d"), i);

            if (!bIsDrawing)
            {
                if (LastDotIndex != DotWidget->DotIndex)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Cannot start drawing from Dot %d, must start from Dot 0"), i);
                    return FReply::Unhandled();
                }

                bIsDrawing = true;
                LastDotIndex = i;

                TemporaryLinePoints.Add(DotImageLocalPosition);
                TemporaryLinePoints.Add(ClickPosition);

                UE_LOG(LogTemp, Warning, TEXT("Started drawing from Dot %d at Local Position: %s"), i, *TemporaryLinePoints.Last().ToString());
            }

            return FReply::Handled();
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Mouse did not click inside any Dot"));
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UDrawingWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FVector2D ReleasePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        if (i == LastDotIndex) // Hindari klik pada Dot yang sama
            continue;

        UDotDrawWidget* DotWidget = DotWidgets[i];

        // Konversi posisi Dot ke ruang lokal widget utama
        FVector2D DotPosition = InGeometry.AbsoluteToLocal(DotWidget->GetCachedGeometry().GetAbsolutePosition());

        float DotLeft = DotPosition.X - DotSize.X / 2;
        float DotRight = DotPosition.X + DotSize.X / 2;
        float DotTop = DotPosition.Y - DotSize.Y / 2;
        float DotBottom = DotPosition.Y + DotSize.Y / 2;

        if (ReleasePosition.X >= DotLeft && ReleasePosition.X <= DotRight &&
            ReleasePosition.Y >= DotTop && ReleasePosition.Y <= DotBottom)
        {
            // Pastikan Dot berikutnya sesuai urutan
            if (i == (LastDotIndex + 1) % DotWidgets.Num())
            {
                FVector2D LastDotPosition = InGeometry.AbsoluteToLocal(DotWidgets[LastDotIndex]->GetCachedGeometry().GetAbsolutePosition());

                // Simpan garis ke daftar
                LinePoints.Add(LastDotPosition); // Tambahkan titik awal
                LinePoints.Add(DotPosition);    // Tambahkan titik tujuan

                TemporaryLinePoints.Empty();
                LastDotIndex = i;

                // Mainkan efek suara
                PlayConnectionSound();

                // Jika kembali ke Dot 0 dan seluruh garis sudah digambar
                if (i == 0 && LinePoints.Num() == DotWidgets.Num() * 2)
                {
                    PlayCompletionSound();
                }
            }
            else
            {
                TemporaryLinePoints.Empty(); // Reset jika urutan salah
            }
            return FReply::Handled();
        }
    }

    TemporaryLinePoints.Empty();
    bIsDrawing = false;
    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UDrawingWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (!bIsDrawing)
        return Super::NativeOnMouseMove(InGeometry, InMouseEvent);

    FVector2D CursorPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    if (LastDotIndex >= 0 && LastDotIndex < DotWidgets.Num())
    {
        TemporaryLinePoints.Empty();
        FVector2D LastDotPosition = DotWidgets[LastDotIndex]->GetCachedGeometry().GetAbsolutePosition();
        TemporaryLinePoints.Add(LastDotPosition);
        TemporaryLinePoints.Add(CursorPosition);
    }
    return FReply::Handled();
}

void UDrawingWidget::SpawnRandomDots(int32 Count)
{
    UE_LOG(LogTemp, Log, TEXT("Dot Spawn"));

    if (!DotDrawWidgetClass || !DrawCanvasPanel || !CanvasBorder) return;

    // Get CanvasBorder size and position
    FVector2D BorderSize = CanvasBorder->GetDesiredSize();
    FGeometry BorderGeometry = CanvasBorder->GetCachedGeometry();
    FVector2D GlobalBorderPosition = BorderGeometry.GetAbsolutePosition();
    float DPIScale = UWidgetLayoutLibrary::GetViewportScale(this);
    GlobalBorderPosition /= DPIScale;

    DotSize = FVector2D(25.0f, 20.0f); // Size of DotDrawWidget
    TArray<FVector2D> ExistingPositions; // Store existing positions

    for (int32 i = 0; i < Count; i++)
    {
        FVector2D RandomPosition;
        bool bIsValidPosition = false;

        // Try generating valid random position
        for (int32 Attempt = 0; Attempt < 100; Attempt++) // Limit attempts to avoid infinite loop
        {
            //float RandomX = FMath::RandRange(0.0f, BorderSize.X - DotSize.X);
            float RandomX = FMath::RandRange(0.0f, 700.f - 20.f);
            //float RandomY = FMath::RandRange(0.0f, BorderSize.Y - DotSize.Y);
            float RandomY = FMath::RandRange(0.0f, 700.f - 25.f);
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
            DotWidget->AddToViewport();
            DotWidget->DotIndex = i;
            DotWidget->SetSequenceText(i + 1);

            // Add to DrawCanvasPanel
            UCanvasPanelSlot* CanvasSlot = DrawCanvasPanel->AddChildToCanvas(DotWidget);
            if (CanvasSlot)
            {
                // Adjust position to be relative to the CanvasBorder's global position
                FVector2D FinalPosition = GlobalBorderPosition + RandomPosition;
                CanvasSlot->SetPosition(RandomPosition);
                CanvasSlot->SetSize(DotSize);

                // Add position to list
                ExistingPositions.Add(FinalPosition);

                // Log the position
                UE_LOG(LogTemp, Log, TEXT("Dot %d - Local RandomPosition: %s, GlobalBorderPosition: %s"), i, *RandomPosition.ToString(), *GlobalBorderPosition.ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to add DotWidget to DrawCanvasPanel."));
            }

            if (i == 0)
            {
                // Set focus to the first dot
                TSharedRef<SWidget> DotWidgetRef = DotWidget->TakeWidget();
                FSlateApplication::Get().SetUserFocus(0, DotWidgetRef, EFocusCause::SetDirectly);

                // Defer task to set mouse location
                FTimerHandle TimerHandle;
                GetWorld()->GetTimerManager().SetTimer(TimerHandle, [DotWidget, GlobalBorderPosition, RandomPosition, this]()
                    {
                        if (!DotWidget)
                        {
                            UE_LOG(LogTemp, Warning, TEXT("DotWidget is no longer valid."));
                            return;
                        }

                        FVector2D FinalCursorPosition = GlobalBorderPosition + RandomPosition;
                        APlayerController* PlayerController = GetOwningPlayer();
                        if (PlayerController)
                        {
                            PlayerController->SetMouseLocation(FinalCursorPosition.X, FinalCursorPosition.Y);
                        }

                    }, 0.01f, false); // Wait a short time for layout to complete
            }
        }
        DotWidgets.Add(DotWidget);
    }
}

void UDrawingWidget::PlayConnectionSound()
{
}

void UDrawingWidget::PlayCompletionSound()
{
}

int32 UDrawingWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int32 CurrentLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    if (DotWidgets.Num() < 2) return CurrentLayer;

    // Dapatkan faktor skala DPI
    float DPIScale = FSlateApplication::Get().GetApplicationScale();

    FLinearColor LineColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);
    float LineThickness = 2.0f;

    // Loop untuk menggambar garis antara titik-titik
    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        const UDotDrawWidget* DotA = DotWidgets[i];
        const UDotDrawWidget* DotB = DotWidgets[(i + 1) % DotWidgets.Num()]; // Menghubungkan ke titik pertama saat i adalah titik terakhir

        if (!DotA || !DotB) continue;

        const UImage* DotImageA = DotA->GetDotImage();
        const UImage* DotImageB = DotB->GetDotImage();

        if (!DotImageA || !DotImageB) continue;

        // Dapatkan posisi absolut dan transformasi dengan DPI scaling
        FVector2D AbsolutePositionA = AllottedGeometry.AbsoluteToLocal(
            DotImageA->GetCachedGeometry().GetAbsolutePosition() / DPIScale
        );
        FVector2D AbsolutePositionB = AllottedGeometry.AbsoluteToLocal(
            DotImageB->GetCachedGeometry().GetAbsolutePosition() / DPIScale
        );

        FVector2D Offset(15.0f, 15.0f);
        AbsolutePositionA += Offset;
        AbsolutePositionB += Offset;

        // Gambar garis antara dua titik
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

    if (LinePoints.Num() >= 2)
    {
        // Pastikan garis utama digambar
        FSlateDrawElement::MakeLines(
            OutDrawElements,
            CurrentLayer,
            AllottedGeometry.ToPaintGeometry(),
            LinePoints,
            ESlateDrawEffect::None,
            FLinearColor::Green, // Warna garis
            true,
            2.0f
        );
    }

    // Garis sementara untuk menggambar garis aktif
    if (TemporaryLinePoints.Num() >= 2)
    {
        // Debugging log untuk memastikan titik-titiknya benar
        for (int32 i = 0; i < TemporaryLinePoints.Num(); ++i)
        {
            UE_LOG(LogTemp, Warning, TEXT("TemporaryLinePoints[%d]: %s"), i, *TemporaryLinePoints[i].ToString());
        }

        FSlateDrawElement::MakeLines(
            OutDrawElements,
            CurrentLayer + 1,
            AllottedGeometry.ToPaintGeometry(),
            TemporaryLinePoints,
            ESlateDrawEffect::None,
            FLinearColor::Red, // Garis sementara
            true,
            2.0f
        );
    }

    //UE_LOG(LogTemp, Warning, TEXT("Drawing Permanent Lines:"));
    //for (const FVector2D& Point : LinePoints)
    //{
    //    // UE_LOG(LogTemp, Warning, TEXT("Point: %s"), *Point.ToString());
    //}

    //UE_LOG(LogTemp, Warning, TEXT("Drawing Temporary Line:"));
    //for (const FVector2D& TempPoint : TemporaryLinePoints)
    //{
    //    // UE_LOG(LogTemp, Warning, TEXT("Temp Point: %s"), *TempPoint.ToString());
    //}

    return CurrentLayer;
}


/*
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
    LastDotIndex = 0;
}

FReply UDrawingWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FVector2D ClickPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        UDotDrawWidget* DotWidget = DotWidgets[i];
        FVector2D DotPosition = InGeometry.AbsoluteToLocal(DotWidget->GetCachedGeometry().GetAbsolutePosition());

        float DotLeft = DotPosition.X - DotSize.X / 2;
        float DotRight = DotPosition.X + DotSize.X / 2;
        float DotTop = DotPosition.Y - DotSize.Y / 2;
        float DotBottom = DotPosition.Y + DotSize.Y / 2;

        if (ClickPosition.X >= DotLeft && ClickPosition.X <= DotRight &&
            ClickPosition.Y >= DotTop && ClickPosition.Y <= DotBottom)
        {
            if (!bIsDrawing)
            {
                bIsDrawing = true;
                LastDotIndex = i;
                TemporaryLinePoints.Add(DotPosition);
            }
            return FReply::Handled();
        }
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UDrawingWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FVector2D ReleasePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        if (i == LastDotIndex)
            continue;

        UDotDrawWidget* DotWidget = DotWidgets[i];
        FVector2D DotPosition = InGeometry.AbsoluteToLocal(DotWidget->GetCachedGeometry().GetAbsolutePosition());

        float DotLeft = DotPosition.X - DotSize.X / 2;
        float DotRight = DotPosition.X + DotSize.X / 2;
        float DotTop = DotPosition.Y - DotSize.Y / 2;
        float DotBottom = DotPosition.Y + DotSize.Y / 2;

        if (ReleasePosition.X >= DotLeft && ReleasePosition.X <= DotRight &&
            ReleasePosition.Y >= DotTop && ReleasePosition.Y <= DotBottom)
        {
            if (i == (LastDotIndex + 1) % DotWidgets.Num())
            {
                FVector2D LastDotPosition = DotWidgets[LastDotIndex]->GetCachedGeometry().GetAbsolutePosition();
                LinePoints.Add(LastDotPosition);
                LinePoints.Add(DotPosition);
                TemporaryLinePoints.Empty();
                LastDotIndex = i;

                // Mainkan efek suara
                PlayConnectionSound();

                // Cek jika berhasil menutup lingkaran
                if (i == 0 && LinePoints.Num() == DotWidgets.Num() * 2)
                {
                    PlayCompletionSound();
                }
            }
            else
            {
                TemporaryLinePoints.Empty();
            }
            return FReply::Handled();
        }
    }

    TemporaryLinePoints.Empty();
    bIsDrawing = false;
    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UDrawingWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (!bIsDrawing)
        return Super::NativeOnMouseMove(InGeometry, InMouseEvent);

    FVector2D CursorPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    if (LastDotIndex >= 0 && LastDotIndex < DotWidgets.Num())
    {
        TemporaryLinePoints.Empty();
        FVector2D LastDotPosition = DotWidgets[LastDotIndex]->GetCachedGeometry().GetAbsolutePosition();
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
    DotSize = FVector2D(25.0f, 20.0f); // Size of DotDrawWidget

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
			DotWidget->AddToViewport();
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

            if (i == 0)
            {
                // Set focus to the first dot
                TSharedRef<SWidget> DotWidgetRef = DotWidget->TakeWidget();
                FSlateApplication::Get().SetUserFocus(0, DotWidgetRef, EFocusCause::SetDirectly);

                // Gunakan Deferred Task untuk menunggu layout selesai
                FTimerHandle TimerHandle;
                GetWorld()->GetTimerManager().SetTimer(TimerHandle, [DotWidget, this]()
                    {
                        if (!DotWidget) return;

                        // Ambil posisi widget di layar
                        FVector2D WidgetAbsolutePosition = DotWidget->GetCachedGeometry().GetAbsolutePosition();

                        // Set posisi kursor ke posisi widget
                        APlayerController* PlayerController = GetOwningPlayer();
                        if (PlayerController)
                        {
							FVector2D WidgetLocation = FVector2D(WidgetAbsolutePosition.X, WidgetAbsolutePosition.Y);
							WidgetLocation = WidgetLocation + FVector2D(5, -30);
                            PlayerController->SetMouseLocation(WidgetLocation.X, WidgetLocation.Y);
                        }
                    }, 0.01f, false); // Tunggu sedikit waktu agar layout selesai
            }
        }
		DotWidgets.Add(DotWidget);
    }
}

void UDrawingWidget::PlayConnectionSound()
{
}

void UDrawingWidget::PlayCompletionSound()
{
}

int32 UDrawingWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int32 CurrentLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    if (DotWidgets.Num() < 2) return CurrentLayer;

    // Dapatkan faktor skala DPI
    float DPIScale = FSlateApplication::Get().GetApplicationScale();

    FLinearColor LineColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);
    float LineThickness = 2.0f;

    // Loop untuk menggambar garis antara titik-titik
    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        const UDotDrawWidget* DotA = DotWidgets[i];
        const UDotDrawWidget* DotB = DotWidgets[(i + 1) % DotWidgets.Num()]; // Menghubungkan ke titik pertama saat i adalah titik terakhir

        if (!DotA || !DotB) continue;

        const UImage* DotImageA = DotA->GetDotImage();
        const UImage* DotImageB = DotB->GetDotImage();

        if (!DotImageA || !DotImageB) continue;

        // Dapatkan posisi absolut dan transformasi dengan DPI scaling
        FVector2D AbsolutePositionA = AllottedGeometry.AbsoluteToLocal(
            DotImageA->GetCachedGeometry().GetAbsolutePosition() / DPIScale
        );
        FVector2D AbsolutePositionB = AllottedGeometry.AbsoluteToLocal(
            DotImageB->GetCachedGeometry().GetAbsolutePosition() / DPIScale
        );

        FVector2D Offset(10.0f, 10.0f);
        AbsolutePositionA += Offset;
        AbsolutePositionB += Offset;

        // Gambar garis antara dua titik
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

    // Gambar garis sementara
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