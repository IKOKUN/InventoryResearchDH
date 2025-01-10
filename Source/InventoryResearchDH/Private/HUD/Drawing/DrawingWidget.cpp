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
#include "Blueprint/SlateBlueprintLibrary.h"
#include "TimerManager.h"
#include "Engine/GameViewportClient.h" // Required for accessing the viewport client


void UDrawingWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	DotParent0->SetSequenceText(1);
	DotParent1->SetSequenceText(2);
	DotParent2->SetSequenceText(3);
	DotParent3->SetSequenceText(4);
	DotParent4->SetSequenceText(5);
	DotParent5->SetSequenceText(6);
}

void UDrawingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Spawn dots sesuai jumlah yang diinginkan
    FTimerHandle SpawnRandomDotTimer;
    GetWorld()->GetTimerManager().SetTimer(SpawnRandomDotTimer, [this]()
        {
            DotParentWidgets.Add(DotParent0);
            DotParentWidgets.Add(DotParent1);
            DotParentWidgets.Add(DotParent2);
            DotParentWidgets.Add(DotParent3);
            DotParentWidgets.Add(DotParent4);
            DotParentWidgets.Add(DotParent5);
		}, 0.1f, false); // Tunggu sebentar agar layout selesai agar nilai dari Geomtery sudah terisi

    //DotParentWidgets.Add(DotParent0);
    //DotParentWidgets.Add(DotParent1);
    //DotParentWidgets.Add(DotParent2);
    //DotParentWidgets.Add(DotParent3);
    //DotParentWidgets.Add(DotParent4);
    //DotParentWidgets.Add(DotParent5);

    LastParentDotIndex = 0;
    bIsDrawing = false;
    ProgressPercentage = 0.0f;
    StartTime = -1.0;
	bProgressCompleted = false;

	//CreatesAllDotChild();

	// Setiap titik akan dihubungkan dengan titik berikutnya
	// Jika sudah mencapai titik terakhir, maka akan kembali ke titik pertama
	// DotCount = 5; // Jumlah titik yang akan digambar

    // Spawn dots sesuai jumlah yang diinginkan
    //  FTimerHandle SpawnRandomDotTimer;
    //  GetWorld()->GetTimerManager().SetTimer(SpawnRandomDotTimer, [this]()
		//{
		//	SpawnRandomDots(DotCount);
		//}, 0.1f, false); // Tunggu sebentar agar layout selesai
}

void UDrawingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update garis sementara jika sedang menggambar
	//if (bIsDrawing)
	//{
		//FVector2D CursorPosition = GetViewportLocalPosition(GetOwningPlayer()->GetMousePosition());
		//UpdateTemporaryLine(CursorPosition);
	//}
}

void UDrawingWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    LinePoints.Empty();
    TemporaryLinePoints.Empty();
    bIsDrawing = false;
    LastParentDotIndex = 0; // Tidak ada Dot aktif pada awalnya
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

    // Periksa apakah klik pada Parent Dot
    for (int32 i = 0; i < DotParentWidgets.Num(); i++)
    {
        UDotDrawWidget* ParentDotWidget = DotParentWidgets[i];
        if (!ParentDotWidget || !ParentDotWidget->DotImage)
            continue;

        // Ambil geometri Parent Dot
        FGeometry ParentDotGeometry = ParentDotWidget->DotImage->GetCachedGeometry();
        FVector2D ParentDotPosition = InGeometry.AbsoluteToLocal(ParentDotGeometry.GetAbsolutePosition());
        FVector2D ParentDotSize = ParentDotGeometry.GetLocalSize();

        // Perhitungan batas Parent Dot
        FVector2D ParentLeftTop = ParentDotPosition - (ParentDotSize * 0.5f);
        FVector2D ParentRightBottom = ParentDotPosition + (ParentDotSize * 0.5f);

        // Tambahkan margin kecil (opsional)
        float Margin = 8.f;
        ParentLeftTop -= FVector2D(Margin, Margin);
        ParentRightBottom += FVector2D(Margin, Margin);

        UE_LOG(LogTemp, Warning, TEXT("Parent Dot %d Position: Center=%s, LeftTop=%s, RightBottom=%s"),
            i, *ParentDotPosition.ToString(), *ParentLeftTop.ToString(), *ParentRightBottom.ToString());

        // Cek apakah klik berada di dalam Parent Dot
        if (ClickPosition.X >= ParentLeftTop.X && ClickPosition.X <= ParentRightBottom.X &&
            ClickPosition.Y >= ParentLeftTop.Y && ClickPosition.Y <= ParentRightBottom.Y)
        {
            if (i != LastParentDotIndex)
            {
                UE_LOG(LogTemp, Warning, TEXT("Cannot click Parent Dot %d out of order! Resetting progress."), i);
                ResetProgress();
                return FReply::Unhandled();
            }

            // Klik Parent Dot yang benar
            bIsDrawing = true;
            LastParentDotIndex = i;
            TemporaryLinePoints.Empty();

            FVector2D ParentDotCenter = ParentDotPosition + ParentDotSize * 0.5f;
            TemporaryLinePoints.Add(ParentDotCenter); // Tambahkan titik awal (Parent Dot)
            LinePoints.Add(ParentDotCenter);

            // Ambil titik untuk Parent Dot berikutnya, pastikan ini adalah titik yang benar
            int32 NextIndex = (i + 1) % DotParentWidgets.Num();
            UDotDrawWidget* NextParentDotWidget = DotParentWidgets[NextIndex];
            FGeometry NextParentDotGeometry = NextParentDotWidget->DotImage->GetCachedGeometry();
            FVector2D NextParentDotPosition = InGeometry.AbsoluteToLocal(NextParentDotGeometry.GetAbsolutePosition());
            FVector2D NextParentDotSize = NextParentDotGeometry.GetLocalSize();
            FVector2D NextParentDotCenter = NextParentDotPosition + NextParentDotSize * 0.5f;
            TemporaryLinePoints.Add(NextParentDotCenter);

            UE_LOG(LogTemp, Warning, TEXT("NextParentDotCenter On Mouse Button Down: %s"), *NextParentDotCenter.ToString());

            StartTime = FPlatformTime::Seconds(); // Mulai progres animasi

            UE_LOG(LogTemp, Warning, TEXT("Started drawing from Parent Dot %d"), LastParentDotIndex);
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
        ProgressPercentage = GetCursorProjectionOnLine(StartPoint, EndPoint, CursorPosition, 100.0f);

        if (ProgressPercentage < 0.0f)
        {
            ProgressPercentage = 0.0f;
        }
        else if (ProgressPercentage > 1.0f)
        {
            ProgressPercentage = 1.0f;
        }

        // Jika kursor mencapai akhir garis (ParentDot selanjutnya), proses pindah ke ParentDot berikutnya
        if (ProgressPercentage >= 1.0f)
        {
            int32 NextIndex = (LastParentDotIndex + 1) % DotParentWidgets.Num(); // Loop kembali ke 0 setelah terakhir
            if (NextIndex >= DotParentWidgets.Num())
            {
                NextIndex = 0; // Mendukung loop dari Dot5 → Dot0
            }

            UDotDrawWidget* NextParentDotWidget = DotParentWidgets[NextIndex];
            if (!NextParentDotWidget || !NextParentDotWidget->DotImage)
            {
                ResetProgress();
                return FReply::Handled();
            }

            // Ambil posisi ParentDot berikutnya
            FGeometry NextParentDotGeometry = NextParentDotWidget->DotImage->GetCachedGeometry();
            FVector2D NextParentDotPosition = InGeometry.AbsoluteToLocal(NextParentDotGeometry.GetAbsolutePosition());
            FVector2D NextParentDotSize = NextParentDotGeometry.GetLocalSize();
            FVector2D NextParentDotCenter = NextParentDotPosition + NextParentDotSize * 0.5f;

            float DistanceToNextParentDot = FVector2D::Distance(CursorPosition, NextParentDotCenter);
            UE_LOG(LogTemp, Warning, TEXT("DistanceToNextParentDot on MouseMove: %f"), DistanceToNextParentDot);

            // Jika kursor berada dalam jarak yang valid dari ParentDot berikutnya
            if (DistanceToNextParentDot <= 50.f) // Threshold jarak
            {
                // Tambahkan titik akhir dari garis sebelumnya ke LinePoints
                if (LinePoints.Num() == 0 || LinePoints.Last() != TemporaryLinePoints[0])
                {
                    LinePoints.Add(TemporaryLinePoints[0]);
                }
                LinePoints.Add(TemporaryLinePoints[1]);

                // Lanjutkan ke ParentDot berikutnya
                LastParentDotIndex = NextIndex; // Perbarui LastParentDotIndex ke indeks berikutnya
                ProgressPercentage = 0.0f;

                // Perbarui TemporaryLinePoints untuk garis baru
                TemporaryLinePoints.Empty();
                TemporaryLinePoints.Add(NextParentDotCenter);

                int32 NextNextIndex = (LastParentDotIndex + 1) % DotParentWidgets.Num();
                UDotDrawWidget* NextNextParentDotWidget = DotParentWidgets[NextNextIndex];
                if (NextNextParentDotWidget && NextNextParentDotWidget->DotImage)
                {
                    FGeometry NextNextParentDotGeometry = NextNextParentDotWidget->DotImage->GetCachedGeometry();
                    FVector2D NextNextParentDotPosition = InGeometry.AbsoluteToLocal(NextNextParentDotGeometry.GetAbsolutePosition());
                    FVector2D NextNextParentDotSize = NextNextParentDotGeometry.GetLocalSize();
                    FVector2D NextNextParentDotCenter = NextNextParentDotPosition + NextNextParentDotSize * 0.5f;

                    TemporaryLinePoints.Add(NextNextParentDotCenter);
                }

                UE_LOG(LogTemp, Warning, TEXT("Moved to next Parent Dot %d"), LastParentDotIndex);
            }
        }

        // Debugging
        UE_LOG(LogTemp, Warning, TEXT("Cursor Progress: %.2f%%"), ProgressPercentage * 100.0f);
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

    // Memeriksa apakah kursor berada dalam jarak yang valid dari ParentDot selanjutnya
    if (LastParentDotIndex + 1 < DotParentWidgets.Num())
    {
        int32 NextIndex = (LastParentDotIndex + 1) % DotParentWidgets.Num();
        UDotDrawWidget* NextParentDotWidget = DotParentWidgets[NextIndex];
        FGeometry NextParentDotGeometry = NextParentDotWidget->DotImage->GetCachedGeometry();
        FVector2D NextParentDotPosition = InGeometry.AbsoluteToLocal(NextParentDotGeometry.GetAbsolutePosition());
        FVector2D NextParentDotSize = NextParentDotGeometry.GetLocalSize();
        FVector2D NextParentDotCenter = NextParentDotPosition + NextParentDotSize * 0.5f;

        // Hitung jarak antara posisi kursor dan titik pusat ParentDot berikutnya
        float DistanceToNextParentDot = FVector2D::Distance(ReleasePosition, NextParentDotCenter);

        UE_LOG(LogTemp, Warning, TEXT("DistanceToNextParentDot: %f"), DistanceToNextParentDot);
        UE_LOG(LogTemp, Warning, TEXT("NextParentDotCenter On Mouse Button Up: %s"), *NextParentDotCenter.ToString());

        // Jika jarak terlalu jauh, hanya reset TemporaryLinePoints, tidak menghapus LinePoints
        if (DistanceToNextParentDot > 40.f) // SomeThreshold adalah jarak maksimum yang diperbolehkan
        {
            UE_LOG(LogTemp, Warning, TEXT("Mouse Release not close enough to next Parent Dot, resetting TemporaryLinePoints only."));
            TemporaryLinePoints.Empty(); // Hanya reset garis sementara
            ProgressPercentage = 0.0f;   // Reset progress percentage
            return FReply::Handled();
        }

        // Lanjutkan ke ParentDot berikutnya jika berada dalam jarak yang valid
        LinePoints.Add(NextParentDotCenter); // Tambahkan titik akhir ke LinePoints

        LastParentDotIndex++;
        ProgressPercentage = 0.0f;

        UE_LOG(LogTemp, Warning, TEXT("Moved to next Parent Dot %d"), LastParentDotIndex);
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
        return -1.0f; // Invalid progress
    }



    return Projection;
}

int32 UDrawingWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int32 CurrentLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    if (DotParentWidgets.Num() < 2) return CurrentLayer;

	DrawPathLine(OutDrawElements, AllottedGeometry, CurrentLayer);
	DrawTemporaryLine(OutDrawElements, AllottedGeometry, CurrentLayer);
    DrawFixLine(OutDrawElements, AllottedGeometry, CurrentLayer);

    //UE_LOG(LogTemp, Warning, TEXT("Drawing Permanent Lines:"));
    //for (const FVector2D& Point : LinePoints)
    //{
    //    // UE_LOG(LogTemp, Warning, TEXT("Point: %s"), *Point.ToString());
    //}

    /*UE_LOG(LogTemp, Warning, TEXT("Drawing Temporary Line:"));
    for (const FVector2D& TempPoint : TemporaryLinePoints)
    {
        UE_LOG(LogTemp, Warning, TEXT("Temp Point: %s"), *TempPoint.ToString());
    }*/

    return CurrentLayer;
}

void UDrawingWidget::DrawPathLine(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 LayerId) const
{
    // Dapatkan faktor skala DPI
    float DPIScale = FSlateApplication::Get().GetApplicationScale();

    FLinearColor LineColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);
    float LineThickness = 2.0f;

    // Loop untuk menggambar garis antara titik-titik
    for (int32 i = 0; i < DotParentWidgets.Num(); i++)
    {
        const UDotDrawWidget* DotA = DotParentWidgets[i];
        const UDotDrawWidget* DotB = DotParentWidgets[(i + 1) % DotParentWidgets.Num()]; // Menghubungkan ke titik pertama saat i adalah titik terakhir

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

        FVector2D OffsetA = DotImageA->GetCachedGeometry().GetAbsoluteSize() / 2;
        FVector2D OffsetB = DotImageB->GetCachedGeometry().GetAbsoluteSize() / 2;
        AbsolutePositionA += OffsetA;
        AbsolutePositionB += OffsetB;

        // Gambar garis antara dua titik
        TArray<FVector2D> LineSegmentPoints = { AbsolutePositionA, AbsolutePositionB };

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
}

void UDrawingWidget::DrawTemporaryLine(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 LayerId) const
{
    // Garis sementara dengan delay
    if (TemporaryLinePoints.Num() >= 2)
    {
        FVector2D TempStart = TemporaryLinePoints[0];
        FVector2D TempEnd = TemporaryLinePoints[1];

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

void UDrawingWidget::DrawFixLine(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, int32 LayerId) const
{
    // Garis sementara untuk menggambar garis aktif
    if (LinePoints.Num() >= 2)
    {
        // Debugging log untuk memastikan titik-titiknya benar
        for (int32 i = 0; i < TemporaryLinePoints.Num(); ++i)
        {
            //UE_LOG(LogTemp, Warning, TEXT("TemporaryLinePoints[%d]: %s"), i, *TemporaryLinePoints[i].ToString());
        }

        FSlateDrawElement::MakeLines(
            OutDrawElements,
            LayerId + 1,
            AllottedGeometry.ToPaintGeometry(),
            LinePoints,
            ESlateDrawEffect::None,
            FLinearColor::Red, // Garis sementara
            true,
            2.0f
        );
    }
}

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
        /*UE_LOG(LogTemp, Log, TEXT("AbsolutePosition: %s, GlobalBorderPosition: %s, LocalPosition: %s"),
            *AbsolutePosition.ToString(), *GlobalBorderPosition.ToString(), *LocalPosition.ToString());*/

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
        UDotDrawWidget* DotWidget = CreateWidget<UDotDrawWidget>(GetWorld(), DotChildWidgetClass);
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
               /* UE_LOG(LogTemp, Log, TEXT("Dot %d - Local RandomPosition: %s, GlobalBorderPosition: %s, FinalPosition: %s"),
                    i, *RandomPosition.ToString(), *GlobalBorderPosition.ToString(), *FinalPosition.ToString());*/
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
        DotParentWidgets.Add(DotWidget);
    }
}

void UDrawingWidget::PlayConnectionSound()
{

}

void UDrawingWidget::PlayCompletionSound()
{

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
