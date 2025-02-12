// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Drawing/DrawingWidget.h"

#include "HUD/Drawing/DotDrawWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/UserInterfaceSettings.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "TimerManager.h"
#include "Engine/GameViewportClient.h" // Required for accessing the viewport client
#include "Math/Vector.h"

void UDrawingWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	DotWidget0->SetSequenceText(1);
	DotWidget1->SetSequenceText(2);
	DotWidget2->SetSequenceText(3);
	DotWidget3->SetSequenceText(4);
	DotWidget4->SetSequenceText(5);
	DotWidget5->SetSequenceText(6);
}

void UDrawingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Spawn dots sesuai jumlah yang diinginkan
    FTimerHandle SpawnRandomDotTimer;
    GetWorld()->GetTimerManager().SetTimer(SpawnRandomDotTimer, [this]()
        {
            DotWidgets.Add(DotWidget0);
            DotWidgets.Add(DotWidget1);
            DotWidgets.Add(DotWidget2);
            DotWidgets.Add(DotWidget3);
            DotWidgets.Add(DotWidget4);
            DotWidgets.Add(DotWidget5);

            // Set First DotImage texture to ConnectionDotTexture
            DotWidget0->SetDotImageTexture(false);


            UImage* DotImage = DotWidget0->GetDotImage();
			if (!DotImage)
			{
				UE_LOG(LogTemp, Warning, TEXT("DotImage is null!"));
				return;
			}
            // Ambil CachedGeometry dari DotImage
            FGeometry DotGeometry = DotImage->GetCachedGeometry();
            // Hitung titik tengah dalam ruang lokal DotImage
            FVector2D DotCenterLocal = DotGeometry.GetLocalSize() * 0.5f;
            // Konversi titik tengah ke posisi absolut (dalam koordinat Slate)
            FVector2D DotCenterAbsolute = DotGeometry.LocalToAbsolute(DotCenterLocal);

            // Dapatkan geometry dari viewport (widget root yang ada di layar)
            FGeometry ViewportGeometry = UWidgetLayoutLibrary::GetViewportWidgetGeometry(this);
            // Hitung posisi titik tengah relatif terhadap viewport
            FVector2D DotCenterViewport = DotCenterAbsolute - ViewportGeometry.GetAbsolutePosition();

            // Log hasilnya untuk debugging
            UE_LOG(LogTemp, Warning, TEXT("DotCenterLocal: %s"), *DotCenterLocal.ToString());
            UE_LOG(LogTemp, Warning, TEXT("DotCenterAbsolute: %s"), *DotCenterAbsolute.ToString());
            UE_LOG(LogTemp, Warning, TEXT("ViewportPosition: %s"), *ViewportGeometry.GetAbsolutePosition().ToString());
            UE_LOG(LogTemp, Warning, TEXT("DotCenterViewport: %s"), *DotCenterViewport.ToString());

            // Set posisi mouse ke titik tersebut
            APlayerController* PlayerController = GetOwningPlayer();
            if (PlayerController)
            {
                PlayerController->SetMouseLocation(DotCenterViewport.X, DotCenterViewport.Y);
                UE_LOG(LogTemp, Warning, TEXT("Mouse positioned to: %s"), *DotCenterViewport.ToString());
            }

        }, 0.3f, false); // Tunggu sebentar agar layout selesai agar nilai dari Geomtery sudah terisi

    LastDotIndex = 0;
    bIsDrawing = false;
    ProgressPercentage = 0.0f;
    StartTime = -1.0;
    bProgressCompleted = false;

    //CreatesAllDotChild();
}

void UDrawingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
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
    //return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(GetWorld()->GetGameViewport()->Viewport->GetSizeXY());
    return FSlateApplication::Get().GetApplicationScale();
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

void UDrawingWidget::ResetProgress()
{
    TemporaryLinePoints.Empty();
    ProgressPercentage = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("Progress reset."));
}

FReply UDrawingWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FVector2D ClickPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
    UE_LOG(LogTemp, Warning, TEXT("Mouse Button Down at: %s"), *ClickPosition.ToString());

	// Periksa apakah klik pada Dot yang valid
    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        UDotDrawWidget* DotWidget = DotWidgets[i];
        if (!DotWidget || !DotWidget->DotImage)
            continue;

        // Ambil geometri  Dot
        FGeometry DotGeometry = DotWidget->DotImage->GetCachedGeometry();
        FVector2D DotPosition = InGeometry.AbsoluteToLocal(DotGeometry.GetAbsolutePosition());
        FVector2D DotSize = DotGeometry.GetLocalSize();

        // Perhitungan batas  Dot
        FVector2D LeftTop = DotPosition - (DotSize * 0.5f);
        FVector2D RightBottom = DotPosition + (DotSize * 0.5f);

        // Tambahkan margin kecil (opsional)
        float Margin = 8.f;
        LeftTop -= FVector2D(Margin, Margin);
        RightBottom += FVector2D(Margin, Margin);

        UE_LOG(LogTemp, Warning, TEXT(" Dot %d Position: Center=%s, LeftTop=%s, RightBottom=%s"),
            i, *DotPosition.ToString(), *LeftTop.ToString(), *RightBottom.ToString());

        // Cek apakah klik berada di dalam  Dot
        if (ClickPosition.X >= LeftTop.X && ClickPosition.X <= RightBottom.X &&
            ClickPosition.Y >= LeftTop.Y && ClickPosition.Y <= RightBottom.Y)
        {
            if (i != LastDotIndex)
            {
                UE_LOG(LogTemp, Warning, TEXT("Cannot click  Dot %d out of order! Resetting progress."), i);
                ResetProgress();
                return FReply::Unhandled();
            }

            // Klik  Dot yang benar
            bIsDrawing = true;
            LastDotIndex = i;
            TemporaryLinePoints.Empty();

            FVector2D DotCenter = DotPosition + DotSize * 0.5f;
            TemporaryLinePoints.Add(DotCenter); // Tambahkan titik awal ( Dot)
            if (LinePoints.Num() == 0 || LinePoints.Last() != DotCenter) {
                LinePoints.Add(DotCenter);
            }
            // LinePoints.Add(DotCenter);

            // Ambil titik untuk  Dot berikutnya, pastikan ini adalah titik yang benar
            int32 NextIndex = (i + 1) % DotWidgets.Num();
            UDotDrawWidget* NextDotWidget = DotWidgets[NextIndex];
            FGeometry NextDotGeometry = NextDotWidget->DotImage->GetCachedGeometry();
            FVector2D NextDotPosition = InGeometry.AbsoluteToLocal(NextDotGeometry.GetAbsolutePosition());
            FVector2D NextDotSize = NextDotGeometry.GetLocalSize();
            FVector2D NextDotCenter = NextDotPosition + NextDotSize * 0.5f;
            TemporaryLinePoints.Add(NextDotCenter);

            UE_LOG(LogTemp, Warning, TEXT("NextDotCenter On Mouse Button Down: %s"), *NextDotCenter.ToString());
            UE_LOG(LogTemp, Warning, TEXT("Klik pada DotWidget %d"), LastDotIndex);
            UE_LOG(LogTemp, Warning, TEXT("Jumlah LinePoints: %d"), LinePoints.Num());

            StartTime = FPlatformTime::Seconds(); // Mulai progres animasi

            UE_LOG(LogTemp, Warning, TEXT("Started drawing from  Dot %d"), LastDotIndex);
            return FReply::Handled();
        }
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UDrawingWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (!bIsDrawing)
    {
        return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
    }

    FVector2D CursorPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    // Menghitung progres berdasarkan posisi kursor di antara dua titik TemporaryLinePoints
    if (TemporaryLinePoints.Num() >= 2)
    {
        FVector2D StartPoint = TemporaryLinePoints[0];
        FVector2D EndPoint = TemporaryLinePoints[1];

        // Hitung progres kursor pada garis
        if (DotWidgets[LastDotIndex]->bIsCurveDot)
        {
            FVector2D ControlPoint1;
            FVector2D ControlPoint2;
            const_cast<UDrawingWidget*>(this)->CalculateControlPoint(ControlPoint1,
                ControlPoint2,
                StartPoint,
                EndPoint,
                DotWidgets[LastDotIndex]->AdditiveDirection1,
                DotWidgets[LastDotIndex]->AdditiveDirection2,
                DotWidgets[LastDotIndex]->bIsRightDirection,
                DotWidgets[LastDotIndex]->bIsDoubleArch,
                DotWidgets[LastDotIndex]->CurveOffset);

			ProgressPercentage = GetCursorProjectionOnCurve(StartPoint, ControlPoint1, ControlPoint2, EndPoint, CursorPosition, ToleranceDrawingDitance);
            if (ProgressPercentage >= 0.0f)
            {
                UE_LOG(LogTemp, Warning, TEXT("Projection valid at t = %f"), ProgressPercentage);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Cursor too far from curve"));
            }
        }  
		else
		{
			ProgressPercentage = GetCursorProjectionOnLine(StartPoint, EndPoint, CursorPosition, ToleranceDrawingDitance);
		}

        if (ProgressPercentage < 0.0f)
        {
            ProgressPercentage = 0.0f;
        }
        else if (ProgressPercentage > 1.0f)
        {
            ProgressPercentage = 1.0f;
        }

        // Jika kursor mencapai akhir garis (Dot selanjutnya), proses pindah ke Dot berikutnya
        if (ProgressPercentage >= 1.0f)
        {
            int32 NextIndex = (LastDotIndex + 1) % DotWidgets.Num(); // Loop kembali ke 0 setelah terakhir
            if (NextIndex >= DotWidgets.Num())
            {
                NextIndex = 0; // Mendukung loop dari Dot5 → Dot0
            }

            UDotDrawWidget* NextDotWidget = DotWidgets[NextIndex];
            if (!NextDotWidget || !NextDotWidget->DotImage)
            {
                ResetProgress();
                return FReply::Handled();
            }

            // Ambil posisi Dot berikutnya
            FGeometry NextDotGeometry = NextDotWidget->DotImage->GetCachedGeometry();
            FVector2D NextDotPosition = InGeometry.AbsoluteToLocal(NextDotGeometry.GetAbsolutePosition());
            FVector2D NextDotSize = NextDotGeometry.GetLocalSize();
            FVector2D NextDotCenter = NextDotPosition + NextDotSize * 0.5f;

            float DistanceToNextDot = FVector2D::Distance(CursorPosition, NextDotCenter);
            UE_LOG(LogTemp, Warning, TEXT("DistanceToNextDot on MouseMove: %f"), DistanceToNextDot);

            // Jika kursor berada dalam jarak yang valid dari Dot berikutnya
            if (DistanceToNextDot <= ToleranceDrawingDitance) // Threshold jarak
            {
                // Tambahkan titik akhir dari garis sebelumnya ke LinePoints
                if (LinePoints.Num() == 0 || LinePoints.Last() != TemporaryLinePoints[0])
                {
                    LinePoints.Add(TemporaryLinePoints[0]);
                }
                LinePoints.Add(TemporaryLinePoints[1]);

                // Lanjutkan ke Dot berikutnya
                LastDotIndex = NextIndex; // Perbarui LastDotIndex ke indeks berikutnya
                ProgressPercentage = 0.0f;

                // Perbarui TemporaryLinePoints untuk garis baru
                TemporaryLinePoints.Empty();
                TemporaryLinePoints.Add(NextDotCenter);

                int32 NextNextIndex = (LastDotIndex + 1) % DotWidgets.Num();
                UDotDrawWidget* NextNextDotWidget = DotWidgets[NextNextIndex];
                if (NextNextDotWidget && NextNextDotWidget->DotImage)
                {
                    FGeometry NextNextDotGeometry = NextNextDotWidget->DotImage->GetCachedGeometry();
                    FVector2D NextNextDotPosition = InGeometry.AbsoluteToLocal(NextNextDotGeometry.GetAbsolutePosition());
                    FVector2D NextNextDotSize = NextNextDotGeometry.GetLocalSize();
                    FVector2D NextNextDotCenter = NextNextDotPosition + NextNextDotSize * 0.5f;

                    TemporaryLinePoints.Add(NextNextDotCenter);
                    
                }
                if (NextIndex == 0)
                {
                    PlayCompletionSound();
                    NextDotWidget->SetDotImageTexture(true);
                    NextDotWidget->PlayDotAnimation();
                }
                else
                {
                    PlayConnectionSound();
					NextDotWidget->SetDotImageTexture(false);
					NextDotWidget->PlayDotAnimation();
                }

                UE_LOG(LogTemp, Warning, TEXT("Moved to next  Dot %d"), LastDotIndex);
            }
        }

        // Debugging
        //UE_LOG(LogTemp, Warning, TEXT("Cursor Progress: %.2f%%"), ProgressPercentage * 100.0f);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TemporaryLinePoints not enough to calculate progress."));
    }

    return FReply::Handled();
}

FReply UDrawingWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (!bIsDrawing)
        return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

    bIsDrawing = false;

    FVector2D ReleasePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
    TemporaryLinePoints.Add(ReleasePosition);

    // Memeriksa apakah kursor berada dalam jarak yang valid dari Dot selanjutnya
    if (LastDotIndex + 1 < DotWidgets.Num())
    {
        int32 NextIndex = (LastDotIndex + 1) % DotWidgets.Num();
        UDotDrawWidget* NextDotWidget = DotWidgets[NextIndex];
        FGeometry NextDotGeometry = NextDotWidget->DotImage->GetCachedGeometry();
        FVector2D NextDotPosition = InGeometry.AbsoluteToLocal(NextDotGeometry.GetAbsolutePosition());
        FVector2D NextDotSize = NextDotGeometry.GetLocalSize();
        FVector2D NextDotCenter = NextDotPosition + NextDotSize * 0.5f;

        // Hitung jarak antara posisi kursor dan titik pusat Dot berikutnya
        float DistanceToNextDot = FVector2D::Distance(ReleasePosition, NextDotCenter);

        UE_LOG(LogTemp, Warning, TEXT("DistanceToNextDot: %f"), DistanceToNextDot);
        UE_LOG(LogTemp, Warning, TEXT("NextDotCenter On Mouse Button Up: %s"), *NextDotCenter.ToString());

        // Jika jarak terlalu jauh, hanya reset TemporaryLinePoints, tidak menghapus LinePoints
        if (DistanceToNextDot > 40.f) // SomeThreshold adalah jarak maksimum yang diperbolehkan
        {
            UE_LOG(LogTemp, Warning, TEXT("Mouse Release not close enough to next  Dot, resetting TemporaryLinePoints only."));
            TemporaryLinePoints.Empty(); // Hanya reset garis sementara
            ProgressPercentage = 0.0f;   // Reset progress percentage
            return FReply::Handled();
        }

        // Lanjutkan ke Dot berikutnya jika berada dalam jarak yang valid
        LinePoints.Add(NextDotCenter); // Tambahkan titik akhir ke LinePoints

        LastDotIndex++;
        ProgressPercentage = 0.0f;

        UE_LOG(LogTemp, Warning, TEXT("Moved to next  Dot %d"), LastDotIndex);
    }

    // Reset TemporaryLinePoints untuk menunggu interaksi berikutnya
    TemporaryLinePoints.Empty();

    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

float UDrawingWidget::GetCursorProjectionOnLine(const FVector2D& LineStart, const FVector2D& LineEnd, const FVector2D& CursorLoc, float Tolerance)
{
    FVector2D LineDirection = LineEnd - LineStart;
    float LineLengthSquared = LineDirection.SizeSquared();

    // Jika panjang garis hampir nol, tidak ada progres
    if (LineLengthSquared < KINDA_SMALL_NUMBER)
    {
        return 0.0f;
    }

    // Proyeksi kursor ke arah garis
    float Projection = FVector2D::DotProduct(CursorLoc - LineStart, LineDirection) / LineLengthSquared;

    // Clamping agar tetap dalam garis (0.0 = LineStart, 1.0 = LineEnd)
    Projection = FMath::Clamp(Projection, 0.0f, 1.0f);

    // Jarak kursor dari garis
    FVector2D ClosestPointOnLine = LineStart + (Projection * LineDirection);
    float DistanceToLine = (CursorLoc - ClosestPointOnLine).Size(); // Perbaikan jarak manual

    // Jika terlalu jauh dari garis, tidak valid
    if (DistanceToLine > Tolerance)
    {
		bIsDrawing = false;
        return -1.0f; // Invalid progress
    }

    return Projection;
}

float UDrawingWidget::GetCursorProjectionOnCurve(const FVector2D& StartPos, const FVector2D& ControlPoint1, const FVector2D& ControlPoint2, const FVector2D& EndPos, const FVector2D& CursorLoc, float Tolerance)
{
    const int32 NumSteps = 100; // Resolusi iterasi
    float ClosestT = -1.0f;
    float MinDistanceSquared = FLT_MAX;

    // Iterasi untuk mencari t optimal
    for (int32 i = 0; i <= NumSteps; i++)
    {
        float t = static_cast<float>(i) / NumSteps;

        // Evaluasi posisi pada kurva menggunakan fungsi Bezier
        FVector2D BezierPoint = FMath::Pow(1 - t, 3) * StartPos +
            3 * FMath::Pow(1 - t, 2) * t * ControlPoint1 +
            3 * (1 - t) * FMath::Pow(t, 2) * ControlPoint2 +
            FMath::Pow(t, 3) * EndPos;

        // Hitung jarak kuadrat ke kursor
        float DistanceSquared = (CursorLoc - BezierPoint).SizeSquared();

        // Perbarui nilai jika jarak lebih kecil
        if (DistanceSquared < MinDistanceSquared)
        {
            MinDistanceSquared = DistanceSquared;
            ClosestT = t;
        }
    }

    // Periksa apakah jarak ke kurva berada dalam toleransi
    if (FMath::Sqrt(MinDistanceSquared) > Tolerance)
    {
        bIsDrawing = false;
        return -1.0f; // Tidak valid
    }

    return ClosestT; // t pada kurva yang paling dekat
}

TArray<FVector2D> UDrawingWidget::SampleBezierCurve(const FVector2D& StartPoint, const FVector2D& ControlPoint1, const FVector2D& ControlPoint2, const FVector2D& EndPoint, float CurrentPercentage, int32 SampleCount)
{
    TArray<FVector2D> SampledPoints;

    // Pastikan variabel SampleCount terdefinisi
    for (int32 i = 0; i <= SampleCount; i++)
    {
        float t = FMath::Clamp(static_cast<float>(i) / SampleCount * CurrentPercentage, 0.0f, 1.0f);
        float OneMinusT = 1.0f - t;

        FVector2D Point =
            OneMinusT * OneMinusT * OneMinusT * StartPoint +
            3 * OneMinusT * OneMinusT * t * ControlPoint1 +
            3 * OneMinusT * t * t * ControlPoint2 +
            t * t * t * EndPoint;

        SampledPoints.Add(Point);
    }

    return SampledPoints;
}

int32 UDrawingWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int32 CurrentLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    if (DotWidgets.Num() < 3) return CurrentLayer;

	DrawPathLine(OutDrawElements, AllottedGeometry, CurrentLayer);
	DrawTemporaryLine(OutDrawElements, AllottedGeometry, CurrentLayer);
    DrawFixLine(OutDrawElements, AllottedGeometry, CurrentLayer);

    UE_LOG(LogTemp, Warning, TEXT("Drawing Permanent Lines:"));
    /*for (const FVector2D& Point : LinePoints)
    {
        UE_LOG(LogTemp, Warning, TEXT("Point: %s"), *Point.ToString());
    }*/

    /*UE_LOG(LogTemp, Warning, TEXT("Drawing Temporary Line:"));
    for (const FVector2D& TempPoint : TemporaryLinePoints)
    {
        UE_LOG(LogTemp, Warning, TEXT("Temp Point: %s"), *TempPoint.ToString());
    }*/

    return CurrentLayer;
}

void UDrawingWidget::CalculateControlPoint(FVector2D& ControlPoint1, FVector2D& ControlPoint2, FVector2D StartPos, FVector2D EndPos, float AdditiveDirection1, float AdditiveDirection2, bool bIsRightDirection, bool bIsDoubleArch, float Offset)
{
    // Tentukan vektor yang tegak lurus terhadap garis antara StartPosition dan EndPosition
    FVector2D Direction = EndPos - StartPos;
    FVector2D Perpendicular = FVector2D(-Direction.Y, Direction.X).GetSafeNormal();

	// Buat titik kontrol berdasarkan arah yang ditentukan
    FVector2D CP1Temp;
	FVector2D CP2Temp;
    if (bIsRightDirection)
    {
        CP1Temp = StartPos + Direction * AdditiveDirection1 + Perpendicular * Offset;
        if (bIsDoubleArch)
        {
            CP2Temp = StartPos + Direction * AdditiveDirection2 - Perpendicular * Offset;
        }
        else
        {
            CP2Temp = StartPos + Direction * AdditiveDirection2 + Perpendicular * Offset;
        }
    }
	else
	{
		CP1Temp = StartPos + Direction * AdditiveDirection1 - Perpendicular * Offset;
		if (bIsDoubleArch)
		{
			CP2Temp = StartPos + Direction * AdditiveDirection2 + Perpendicular * Offset;
		}
        else
        {
            CP2Temp = StartPos + Direction * AdditiveDirection2 - Perpendicular * Offset;
        }
	}
	ControlPoint1 = CP1Temp;
	ControlPoint2 = CP2Temp;
}

void UDrawingWidget::DrawPathLine(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 LayerId) const
{
    // Dapatkan faktor skala DPI
    float DPIScale = FSlateApplication::Get().GetApplicationScale();

    const float CurveTension = 0.5f; // Nilai default untuk kelengkungan
    FLinearColor LineColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);
    float LineThickness = 2.0f;

    // Loop untuk menggambar garis antara titik-titik
    for (int32 i = 0; i < DotWidgets.Num(); i++)
    {
        const UDotDrawWidget* CurrentDot = DotWidgets[i];
        const UDotDrawWidget* NextDot = DotWidgets[(i + 1) % DotWidgets.Num()]; // Menghubungkan ke titik pertama saat i adalah titik terakhir

        if (!CurrentDot || !NextDot) continue;

        const UImage* CurrentDotImage = CurrentDot->GetDotImage();
        const UImage* NextDotImage = NextDot->GetDotImage();

        if (!CurrentDotImage || !NextDotImage) continue;


        // Dapatkan posisi absolut dan transformasi dengan DPI scaling
        FVector2D StartPosition = AllottedGeometry.AbsoluteToLocal(
            CurrentDotImage->GetCachedGeometry().GetAbsolutePosition()
        );
        FVector2D EndPosition = AllottedGeometry.AbsoluteToLocal(
            NextDotImage->GetCachedGeometry().GetAbsolutePosition()
        );

        FVector2D CurrentOffset = CurrentDotImage->GetCachedGeometry().GetLocalSize() / 2;
        FVector2D NextOffset = NextDotImage->GetCachedGeometry().GetLocalSize() / 2;

        StartPosition += CurrentOffset;
        EndPosition += NextOffset;


        if (CurrentDot->bIsCurveDot)
        {
            //// Tentukan vektor yang tegak lurus terhadap garis antara StartPosition dan EndPosition
            //FVector2D Direction = EndPosition - StartPosition;
            //FVector2D Perpendicular = FVector2D(-Direction.Y, Direction.X).GetSafeNormal();

            //// Tambahkan offset pada titik kontrol berdasarkan vektor tegak lurus
            //float CurveOffset = 300.0f; // Ubah sesuai kebutuhan
            //FVector2D ControlPoint1 = StartPosition + Direction * 0.33f + Perpendicular * CurveOffset;
            //FVector2D ControlPoint2 = StartPosition + Direction * 0.66f - Perpendicular * CurveOffset;

			FVector2D ControlPoint1;
			FVector2D ControlPoint2;
            const_cast<UDrawingWidget*>(this)->CalculateControlPoint(ControlPoint1, 
                ControlPoint2, 
                StartPosition, 
                EndPosition, 
                CurrentDot->AdditiveDirection1, 
                CurrentDot->AdditiveDirection2, 
                CurrentDot->bIsRightDirection, 
                CurrentDot->bIsDoubleArch, 
                CurrentDot->CurveOffset);

            // Gambar kurva menggunakan MakeCubicBezierSpline
            FSlateDrawElement::MakeCubicBezierSpline(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(),
                StartPosition,
                ControlPoint1,
                ControlPoint2,
                EndPosition,
                LineThickness,
                ESlateDrawEffect::None,
                LineColor
            );

            /*FSlateDrawElement::MakeSpline(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(),
                StartPosition,
                ControlPoint1,
                ControlPoint2,
                EndPosition,
                LineThickness,
                ESlateDrawEffect::None,
                LineColor
            );*/

            //UE_LOG(LogTemp, Warning, TEXT("Start Position: %s"), *StartPosition.ToString());
            //UE_LOG(LogTemp, Warning, TEXT("End Position: %s"), *EndPosition.ToString());
            //UE_LOG(LogTemp, Warning, TEXT("ControlPoint1: %s"), *ControlPoint1.ToString());
            //UE_LOG(LogTemp, Warning, TEXT("ControlPoint2: %s"), *ControlPoint2.ToString());

        }
        else
        {
            // Gambar garis lurus
            TArray<FVector2D> LineSegmentPoints = { StartPosition, EndPosition };
            FSlateDrawElement::MakeLines(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(),
                LineSegmentPoints,
                ESlateDrawEffect::None,
                LineColor,
                true,
                LineThickness
            );
        }

        // Gambar garis antara dua titik
        /*TArray<FVector2D> LineSegmentPoints = {CurrentAbsolutePosition, NextAbsolutePosition};

        FSlateDrawElement::MakeLines(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(),
            LineSegmentPoints,
            ESlateDrawEffect::None,
            LineColor,
            true,
            LineThickness
        );*/

        // Lengkungan (2) For Testing
  //      FVector2D StartPoint2(100.0f, 150.0f); // Titik awal
  //      FVector2D ControlPoint1(120.0f, 200.0f); // Kontrol titik 1
  //      FVector2D ControlPoint2(150.0f, 200.0f); // Kontrol titik 2
  //      FVector2D EndPoint2(200.0f, 150.0f); // Titik akhir
  //      FLinearColor LineColor2 = FLinearColor::Black;

		//FSlateDrawElement::MakeCubicBezierSpline(
		//	OutDrawElements,
		//	LayerId,
		//	AllottedGeometry.ToPaintGeometry(),
  //          StartPoint2,
		//	ControlPoint1,
		//	ControlPoint2,
		//	EndPoint2,
		//	LineThickness, // Ketebalan garis
		//	ESlateDrawEffect::None,
		//	LineColor
		//);
    }
}

void UDrawingWidget::DrawTemporaryLine(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 LayerId) const
{
    // Garis sementara dengan delay
    if (TemporaryLinePoints.Num() >= 2)
    {
        FVector2D TempStart = TemporaryLinePoints[0];
        FVector2D TempEnd = TemporaryLinePoints[1];

        if (DotWidgets[LastDotIndex]->bIsCurveDot)
        {
            //// Tentukan vektor yang tegak lurus terhadap garis antara StartPosition dan EndPosition
            //FVector2D Direction = TempEnd - TempStart;
            //FVector2D Perpendicular = FVector2D(-Direction.Y, Direction.X).GetSafeNormal();

            //float CurveOffset = 300.0f; // Ubah sesuai kebutuhan
            //FVector2D ControlPoint1 = TempStart + Direction * 0.33f + Perpendicular * CurveOffset;
            //FVector2D ControlPoint2 = TempStart + Direction * 0.66f - Perpendicular * CurveOffset;

            FVector2D ControlPoint1;
            FVector2D ControlPoint2;
            const_cast<UDrawingWidget*>(this)->CalculateControlPoint(ControlPoint1,
                ControlPoint2,
                TempStart,
                TempEnd,
                DotWidgets[LastDotIndex]->AdditiveDirection1,
                DotWidgets[LastDotIndex]->AdditiveDirection2,
                DotWidgets[LastDotIndex]->bIsRightDirection,
                DotWidgets[LastDotIndex]->bIsDoubleArch,
                DotWidgets[LastDotIndex]->CurveOffset);


            // Panggil fungsi untuk mengambil sampel titik-titik
            TArray<FVector2D> SampledPoints = const_cast<UDrawingWidget*>(this)->SampleBezierCurve(TempStart, ControlPoint1, ControlPoint2, TempEnd, ProgressPercentage, 100);


            // Gunakan SampledPoints untuk menggambar garis
            FSlateDrawElement::MakeLines(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(),
                SampledPoints,
                ESlateDrawEffect::None,
                FLinearColor::Blue,
                true,
                2.0f
            );

            // Hitung titik akhir progres berdasarkan ProgressPercentage
            //FVector2D AnimatedEndPoint = FMath::Lerp(TempStart, TempEnd, ProgressPercentage);

            //// Gambar kurva menggunakan MakeCubicBezierSpline
            //FSlateDrawElement::MakeCubicBezierSpline(
            //    OutDrawElements,
            //    LayerId,
            //    AllottedGeometry.ToPaintGeometry(),
            //    TempStart,
            //    ControlPoint1,
            //    ControlPoint2,
            //    TempEnd,
            //    2.0f,
            //    ESlateDrawEffect::None,
            //    FLinearColor::Blue
            //);

            //UE_LOG(LogTemp, Warning, TEXT("Start Position: %s"), *StartPosition.ToString());
            //UE_LOG(LogTemp, Warning, TEXT("End Position: %s"), *EndPosition.ToString());
            //UE_LOG(LogTemp, Warning, TEXT("ControlPoint1: %s"), *ControlPoint1.ToString());
            //UE_LOG(LogTemp, Warning, TEXT("ControlPoint2: %s"), *ControlPoint2.ToString());

        }
        else
        {
            // Validasi ProgressPercentage
            if (ProgressPercentage < 0.0f || ProgressPercentage > 1.0f)
            {
                UE_LOG(LogTemp, Warning, TEXT("Invalid Progress Percentage!"));
                return;
            }

            // Hitung titik akhir progres berdasarkan ProgressPercentage
            FVector2D AnimatedEndPoint = FMath::Lerp(TempStart, TempEnd, ProgressPercentage);

            // Gambar garis progresif
            TArray<FVector2D> ProgressiveLine = { TempStart, AnimatedEndPoint };

            FSlateDrawElement::MakeLines(
                OutDrawElements,
                LayerId + 1,
                AllottedGeometry.ToPaintGeometry(),
                ProgressiveLine,
                ESlateDrawEffect::None,
                FLinearColor::Blue, // Warna garis progresif
                true,
                2.0f
            );
        }      
    }
}

void UDrawingWidget::DrawFixLine(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 LayerId) const
{
    if (DotWidgets.Num() < LinePoints.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("DotWidgets and LinePoints size mismatch!"));
        return;
    }

    // Garis sementara untuk menggambar garis aktif
    if (LinePoints.Num() >= 2)
    {
        for (int32 i = 0; i < LinePoints.Num(); i++)
        {
            if (i - 1 >= 0)
            {
                FVector2D StartPosition = LinePoints[i - 1];
                FVector2D EndPosition = LinePoints[i];

                if (DotWidgets.IsValidIndex(i - 1) && DotWidgets[i - 1]->bIsCurveDot) // Validasi index DotWidgets
                {
                    //// Tentukan vektor yang tegak lurus terhadap garis antara StartPosition dan EndPosition
                    //FVector2D Direction = EndPosition - StartPosition;
                    //FVector2D Perpendicular = FVector2D(-Direction.Y, Direction.X).GetSafeNormal();

                    //// Tambahkan offset pada titik kontrol berdasarkan vektor tegak lurus
                    //float CurveOffset = 300.0f; // Ubah sesuai kebutuhan
                    //FVector2D ControlPoint1 = StartPosition + Direction * 0.33f + Perpendicular * CurveOffset;
                    //FVector2D ControlPoint2 = StartPosition + Direction * 0.66f - Perpendicular * CurveOffset;

                    FVector2D ControlPoint1;
                    FVector2D ControlPoint2;
                    const_cast<UDrawingWidget*>(this)->CalculateControlPoint(ControlPoint1,
                        ControlPoint2,
                        StartPosition,
                        EndPosition,
                        DotWidgets[i]->AdditiveDirection1,
                        DotWidgets[i]->AdditiveDirection2,
                        DotWidgets[i]->bIsRightDirection,
                        DotWidgets[i]->bIsDoubleArch,
                        DotWidgets[i]->CurveOffset);

                    // Gambar kurva menggunakan MakeCubicBezierSpline
                    FSlateDrawElement::MakeCubicBezierSpline(
                        OutDrawElements,
                        LayerId + 1,
                        AllottedGeometry.ToPaintGeometry(),
                        StartPosition,
                        ControlPoint1,
                        ControlPoint2,
                        EndPosition,
                        2.f,
                        ESlateDrawEffect::None,
                        FLinearColor::Red
                    );

                    //UE_LOG(LogTemp, Warning, TEXT("Start Position: %s"), *StartPosition.ToString());
                    //UE_LOG(LogTemp, Warning, TEXT("End Position: %s"), *EndPosition.ToString());
                    //UE_LOG(LogTemp, Warning, TEXT("ControlPoint1: %s"), *ControlPoint1.ToString());
                    //UE_LOG(LogTemp, Warning, TEXT("ControlPoint2: %s"), *ControlPoint2.ToString());

                }
                else if (DotWidgets.IsValidIndex(i - 1)) // Validasi jika bukan kurva
                {
                    // Gambar garis lurus
                    TArray<FVector2D> LineSegmentPoints = { StartPosition, EndPosition };
                    FSlateDrawElement::MakeLines(
                        OutDrawElements,
                        LayerId + 1,
                        AllottedGeometry.ToPaintGeometry(),
                        LineSegmentPoints,
                        ESlateDrawEffect::None,
                        FLinearColor::Red,
                        true,
                        2.f
                    );
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Invalid DotWidgets index: %d"), i - 1);
                }
            }			
        }
        //FSlateDrawElement::MakeLines(
        //    OutDrawElements,
        //    LayerId + 1,
        //    AllottedGeometry.ToPaintGeometry(),
        //    LinePoints,
        //    ESlateDrawEffect::None,
        //    FLinearColor::Red, // Garis sementara
        //    true,
        //    2.0f
        //);
    }
}

void UDrawingWidget::PlayConnectionSound()
{
    if (ConnectionSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), ConnectionSound);
    }
}

void UDrawingWidget::PlayCompletionSound()
{
    if (CompletionSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), CompletionSound);
    }
}

void UDrawingWidget::UpdateTemporaryLinePoints()
{
    if (TemporaryLinePoints.Num() >= 2)
    {
        double CurrentTime = FPlatformTime::Seconds();
        double ElapsedTime = FMath::Max(CurrentTime - StartTime, 0.0);

        float AnimationProgress = FMath::Clamp(ElapsedTime / 0.2f, 0.0f, 1.0f);

        if (AnimationProgress >= 1.0f)
        {
            LinePoints.Add(TemporaryLinePoints.Last()); // Tambahkan titik terakhir
            TemporaryLinePoints.Empty(); // Reset TemporaryLinePoints
            StartTime = -1.0; // Reset waktu
        }
    }
}

/*
void UDrawingWidget::SpawnRandomDots(int32 Count)
{

    UE_LOG(LogTemp, Log, TEXT("Dot Spawn"));

    if (!DrawCanvasPanel || !CanvasBorder) return;

    // Get DPI scale
   // float DPIScale = UWidgetLayoutLibrary::GetViewportScale(this);
    float DPIScale = GetDPIScale();

    // Get CanvasBorder size and geometry
    FGeometry BorderGeometry = CanvasBorder->GetCachedGeometry();
    FVector2D BorderSize = BorderGeometry.GetAbsoluteSize();

    FVector2D BorderTopLeftPosition = USlateBlueprintLibrary::GetLocalTopLeft(BorderGeometry);


    // Get Global Position of CanvasBorder in Screen Space
    FVector2D AbsolutePosition = BorderGeometry.GetAbsolutePosition();
    FVector2D GlobalBorderPosition = AbsolutePosition / DPIScale;

    DotSize = FVector2D(25.0f, 20.0f); // Size of DotDrawWidget
    TArray<FVector2D> ExistingPositions; // Store existing positions
    if (CanvasBorder)
    {
        // Update geometry to ensure we're using the latest
        BorderGeometry = CanvasBorder->GetCachedGeometry();
        FVector2D PixelViewport;
        FVector2D ViewportPosition;
        USlateBlueprintLibrary::LocalToViewport(this, BorderGeometry, FVector2D::ZeroVector, PixelViewport, ViewportPosition);
        // Get the Absolute position from the geometry
        AbsolutePosition = BorderGeometry.GetAbsolutePosition();

        // Adjust the Absolute Position based on DPI scale
        GlobalBorderPosition = AbsolutePosition / DPIScale;



        // Log the results
        UE_LOG(LogTemp, Log, TEXT("AbsolutePosition: %s, GlobalBorderPosition: %s, LocalPosition: %s"),
            *AbsolutePosition.ToString(), *GlobalBorderPosition.ToString(), *LocalPosition.ToString());

            // Log the results
        UE_LOG(LogTemp, Log, TEXT("PixelViewport: %s, ViewportPosition: %s, BorderTopLeftPosition : %s"),
            *PixelViewport.ToString(), *ViewportPosition.ToString(), *BorderTopLeftPosition.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CanvasBorder is null!"));
    }

    for (int32 i = 0; i < Count; i++)
    {
        FVector2D RandomPosition;
        bool bIsValidPosition = false;

        for (int32 Attempt = 0; Attempt < 100; Attempt++) // Limit attempts to avoid infinite loop
        {
            float RandomX = FMath::RandRange(0.0f, static_cast<float>(BorderSize.X - DotSize.X));
            float RandomY = FMath::RandRange(0.0f, static_cast<float>(BorderSize.Y - DotSize.Y));
            //UE_LOG(LogTemp, Log, TEXT("BorderSizeX: %f, BorderSizeY: %f"), BorderSize.X, BorderSize.Y);
            //UE_LOG(LogTemp, Log, TEXT("DotSizeX: %f, DotSizeY: %f"), DotSize.X, DotSize.Y);
            //UE_LOG(LogTemp, Log, TEXT("RandomX: %f, RandomY: %f"), RandomX, RandomY);
            //float RandomX = FMath::RandRange(0.0f, 700.0f - 20.0f); // Pastikan semua literal bertipe float
            //float RandomY = FMath::RandRange(0.0f, 700.0f - 25.0f); // Literal float digunakan di sini
            RandomPosition = FVector2D(RandomX, RandomY);

            bIsValidPosition = true;

            // Check for overlap with existing positions
            for (const FVector2D& ExistingPosition : ExistingPositions)
            {
                // Ensure distance is greater than dot size to avoid overlap
                if (FVector2D::DistSquared(RandomPosition, ExistingPosition) < FMath::Square(DotSize.X))
                {
                    bIsValidPosition = false;
                    break;
                }
            }

            if (bIsValidPosition) break;
        }

        if (!bIsValidPosition)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find valid position for dot %d after 100 attempts."), i);
            continue; // Skip this dot if no valid position found
        }

        // Log positions
        //UE_LOG(LogTemp, Log, TEXT("Dot %d: RandomPosition (Local): %s"), i, *RandomPosition.ToString());
        //UE_LOG(LogTemp, Log, TEXT("Dot %d: CanvasBorder Global Position: %s"), i, *GlobalBorderPosition.ToString());

        FVector2D FinalPosition = GlobalBorderPosition + RandomPosition;
        //UE_LOG(LogTemp, Log, TEXT("Dot %d: FinalPosition (Global): %s"), i, *FinalPosition.ToString());

        // Create DotDrawWidget
        //UDotDrawWidget* DotWidget = CreateWidget<UDotDrawWidget>(GetWorld(), DotChildWidgetClass);
        UDotDrawWidget* DotWidget = nullptr;
        if (DotWidget)
        {
            DotWidget->AddToViewport();
            DotWidget->DotIndex = i;
            DotWidget->SetSequenceText(i + 1);

            // Set Position Dot With Timer
            // this function is need because if we set the position directly, the geometry of the widget is not yet updated
            // so we need to wait a little bit for the layout to complete
            //FTimerHandle SetPositionDotTimer;
   //         GetWorld()->GetTimerManager().SetTimer(SetPositionDotTimer, [DotWidget, this]()
   //             {
   //                 //DotWidget->DotImage.
   //             }, 0.1f, false); // Tunggu sebentar agar layout selesai

            UE_LOG(LogTemp, Log, TEXT("Dot Widget Successfully Spawn"));
            // Add to DrawCanvasPanel
            UCanvasPanelSlot* CanvasSlot = DrawCanvasPanel->AddChildToCanvas(DotWidget);
            if (CanvasSlot)
            {
                // Adjust position to be relative to the CanvasBorder's global position
                CanvasSlot->SetPosition(FinalPosition);
                CanvasSlot->SetSize(DotSize);

                // Add position to list
                ExistingPositions.Add(FinalPosition);

                // Log the position
               UE_LOG(LogTemp, Log, TEXT("Dot %d - Local RandomPosition: %s, GlobalBorderPosition: %s, FinalPosition: %s"),
                    i, *RandomPosition.ToString(), *GlobalBorderPosition.ToString(), *FinalPosition.ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to add DotWidget to DrawCanvasPanel."));
            }

            if (i == 0)
            {
                // Set mouse location ke posisi DotWidget pertama
                FTimerHandle TimerHandle;
                GetWorld()->GetTimerManager().SetTimer(TimerHandle, [DotWidget, DPIScale, this]()
                    {
                        // Set focus to the first dot
                        TSharedRef<SWidget> DotWidgetRef = DotWidget->TakeWidget();
                        FSlateApplication::Get().SetUserFocus(0, DotWidgetRef, EFocusCause::SetDirectly);

                        // Ambil Geometry dari DotWidget
                        FGeometry DotGeometry = DotWidget->GetCachedGeometry();
                        FVector2D DotPosition = DotGeometry.GetAbsolutePosition();
                        FVector2D DotSize = DotGeometry.GetAbsoluteSize();
                        FVector2D DotCenter = DotPosition + DotSize * 0.5f;

                        // Sesuaikan dengan DPI scaling
                        FVector2D DotCenterScaled = DotCenter / (DPIScale + 0.088f) + FVector2D(0.f, -33.f);

                        UE_LOG(LogTemp, Log, TEXT("DPIScale: %f"), DPIScale);

                        // Tambahkan offset window (untuk mode windowed)
                        if (GEngine && GEngine->GameViewport)
                        {
                            TSharedPtr<SWindow> Window = GEngine->GameViewport->GetWindow();
                            if (Window.IsValid())
                            {
                                FVector2D WindowPositionDesktop = Window->GetPositionInScreen();
                                DotCenterScaled += WindowPositionDesktop;
                            }
                        }

                        // Log untuk debugging
                        UE_LOG(LogTemp, Log, TEXT("DotWidget[0] Absolute Position: %s"), *DotPosition.ToString());
                        UE_LOG(LogTemp, Log, TEXT("DotWidget[0] Size: %s"), *DotSize.ToString());
                        UE_LOG(LogTemp, Log, TEXT("DotWidget[0] Center Position: %s"), *DotCenter.ToString());

                        APlayerController* PlayerController = GetOwningPlayer();
                        if (PlayerController)
                        {
                            PlayerController->SetMouseLocation(DotCenterScaled.X, DotCenterScaled.Y);
                        }
                    }, 1.f, false); // Tunggu agar layout selesai
            }
        }
        DotWidgets.Add(DotWidget);
    }

}
*/
