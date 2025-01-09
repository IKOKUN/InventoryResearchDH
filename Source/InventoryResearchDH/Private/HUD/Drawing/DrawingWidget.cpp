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

    DotParentWidgets.Add(DotParent0);
    DotParentWidgets.Add(DotParent1);
    DotParentWidgets.Add(DotParent2);
    DotParentWidgets.Add(DotParent3);
    DotParentWidgets.Add(DotParent4);
    DotParentWidgets.Add(DotParent5);

    LastParentDotIndex = 0;
    LastChildDotIndex = 0;
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
	LastChildDotIndex = 0; // Tidak ada Dot aktif pada awalnya
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

void UDrawingWidget::CreatesAllDotChild()
{
    UE_LOG(LogTemp, Log, TEXT("Creating DotChilds"));
    if (DotParentWidgets.Num() < 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough DotParent widgets to create connections."));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("Creating DotChilds 1"));
    const int32 NumDotsPerSegment = 10; // Jumlah DotChild per segmen
    const FVector2D DotChildSize(30.0f, 20.f); // Ukuran DotChild (X=20, Y=10)

    // Loop melalui semua DotParent untuk membuat garis tertutup (loop 0-1-2-3-4-5-0)
    for (int32 i = 0; i < DotParentWidgets.Num(); i++)
    {
        UDotDrawWidget* StartDotParent = DotParentWidgets[i];
        UDotDrawWidget* EndDotParent = DotParentWidgets[(i + 1) % DotParentWidgets.Num()]; // Wrap ke index awal

        if (!StartDotParent || !EndDotParent)
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid DotParent at index %d or %d"), i, (i + 1) % DotParentWidgets.Num());
            continue;
        }

        // Dapatkan posisi awal dan akhir dalam koordinat lokal
        FVector2D StartPos, EndPos;

        if (UCanvasPanelSlot* StartSlot = Cast<UCanvasPanelSlot>(StartDotParent->Slot))
        {
            StartPos = StartSlot->GetPosition();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("StartDotParent is not using a CanvasPanelSlot."));
            continue;
        }

        if (UCanvasPanelSlot* EndSlot = Cast<UCanvasPanelSlot>(EndDotParent->Slot))
        {
            EndPos = EndSlot->GetPosition();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("EndDotParent is not using a CanvasPanelSlot."));
            continue;
        }

        // Buat DotChild sepanjang garis antara StartPos dan EndPos
        for (int32 j = 1; j <= NumDotsPerSegment; j++) // Mulai dari 1 untuk menghindari spawn di posisi StartDotParent
        {
            // Interpolasi posisi DotChild
            float Alpha = static_cast<float>(j) / (NumDotsPerSegment + 1); // Alpha dari 0.1 ke 0.9
            FVector2D ChildPos = FMath::Lerp(StartPos, EndPos, Alpha);

            // Spawn widget DotChild
            UDotDrawWidget* DotChild = CreateWidget<UDotDrawWidget>(this, DotChildWidgetClass); // DotChildClass harus di-set sebelumnya
            if (DotChild)
            {
                DotChild->AddToViewport();
                UCanvasPanelSlot* ChildSlot = DrawCanvasPanel->AddChildToCanvas(DotChild);
                if (ChildSlot)
                {
                    ChildSlot->SetPosition(ChildPos); // Atur posisi DotChild
                    ChildSlot->SetSize(DotChildSize); // Atur ukuran DotChild

                    // Set properti DotChild
                    DotChild->bIsParent = false;
                    DotChild->ParentDotIndex = i; // Set ParentDotIndex ke StartDotParent index
                    DotChild->DotIndex = j-1; // Set DotIndex sesuai urutan
                    DotChild->SetSequenceText(j); // Set urutan sesuai index
                    DotChildWidgets.Add(DotChild); // Tambahkan ke array DotChildWidgets

                    // UE_LOG(LogTemp, Log, TEXT("DotChild spawned at position %s"), *ChildPos.ToString());
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("DotChild is not using a CanvasPanelSlot."));
                }
            }
        }
    }

}

void UDrawingWidget::ResetProgress()
{
	LastParentDotIndex = 0;
	LastChildDotIndex = 0;
	bIsDrawing = false;
	LinePoints.Empty();
	TemporaryLinePoints.Empty();
	StartTime = -1.0;
	bProgressCompleted = false;
	ProgressPercentage = 0.0f;
	UE_LOG(LogTemp, Warning, TEXT("Progress reset."));
}

UDotDrawWidget* UDrawingWidget::GetDotParentWidgetFromIndex(int32 Index)
{
	if (Index >= 0 && Index < DotParentWidgets.Num())
	{
		return DotParentWidgets[Index];
	}
    return nullptr;
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
            LastChildDotIndex = 0;
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
        if (ProgressPercentage >= 1.0f && LastParentDotIndex + 1 < DotParentWidgets.Num())
        {
            int32 NextIndex = (LastParentDotIndex + 1) % DotParentWidgets.Num();
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
            if (DistanceToNextParentDot <= 100.f) // Threshold jarak
            {
                // Lanjutkan ke ParentDot berikutnya
                LinePoints.Add(NextParentDotCenter);
                LastParentDotIndex++;
                LastChildDotIndex = 0;
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


        // Jika jarak terlalu jauh, reset drawing
        if (DistanceToNextParentDot > 100.f) // SomeThreshold adalah jarak maksimum yang diperbolehkan
        {
            UE_LOG(LogTemp, Warning, TEXT("Mouse Release not close enough to next Parent Dot, resetting."));
            ResetProgress();
            return FReply::Handled();
        }

        // Lanjutkan ke ParentDot berikutnya jika berada dalam jarak yang valid
        LinePoints.Add(NextParentDotCenter);
        LastParentDotIndex++;
        LastChildDotIndex = 0;
        ProgressPercentage = 0.0f;

        UE_LOG(LogTemp, Warning, TEXT("Moved to next Parent Dot %d"), LastParentDotIndex);
    }

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
            DotImageA->GetCachedGeometry().GetAbsolutePosition() * DPIScale
        );
        FVector2D AbsolutePositionB = AllottedGeometry.AbsoluteToLocal(
            DotImageB->GetCachedGeometry().GetAbsolutePosition() * DPIScale
        );

        FVector2D OffsetA = DotImageA->GetCachedGeometry().GetAbsoluteSize() / 2;
        FVector2D OffsetB = DotImageB->GetCachedGeometry().GetAbsoluteSize() / 2;
        AbsolutePositionA += OffsetA;
        AbsolutePositionB += OffsetB;

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

    // Garis sementara dengan delay
    if (TemporaryLinePoints.Num() >= 2)
    {
        FVector2D TempStart = TemporaryLinePoints[0];
        FVector2D TempEnd = TemporaryLinePoints[1];

        // Validasi ProgressPercentage
        if (ProgressPercentage < 0.0f || ProgressPercentage > 1.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid Progress Percentage!"));
            return CurrentLayer; // Tidak menggambar jika ProgressPercentage invalid
        }

        // Hitung titik akhir progres berdasarkan ProgressPercentage
        FVector2D AnimatedEndPoint = FMath::Lerp(TempStart, TempEnd, ProgressPercentage);

        // Gambar garis progresif
        TArray<FVector2D> ProgressiveLine = { TempStart, AnimatedEndPoint };

        FSlateDrawElement::MakeLines(
            OutDrawElements,
            CurrentLayer + 1,
            AllottedGeometry.ToPaintGeometry(),
            ProgressiveLine,
            ESlateDrawEffect::None,
            FLinearColor::Blue, // Warna garis progresif
            true,
            2.0f
        );
    }

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

    /*UE_LOG(LogTemp, Warning, TEXT("Drawing Temporary Line:"));
    for (const FVector2D& TempPoint : TemporaryLinePoints)
    {
        UE_LOG(LogTemp, Warning, TEXT("Temp Point: %s"), *TempPoint.ToString());
    }*/

    return CurrentLayer;
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



/*
FReply UDrawingWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (!bIsDrawing)
        return Super::NativeOnMouseMove(InGeometry, InMouseEvent);

    FVector2D CursorPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    // Cek apakah kursor melewati Child Dot
    for (int32 i = 0; i < DotChildWidgets.Num(); i++)
    {
        UDotDrawWidget* ChildDotWidget = DotChildWidgets[i];
        if (!ChildDotWidget || !ChildDotWidget->DotImage)
            continue;

        // Periksa apakah Child Dot milik Parent Dot yang sedang aktif
        if (ChildDotWidget->ParentDotIndex != LastParentDotIndex)
            continue;

        // Ambil geometri Child Dot
        FGeometry ChildDotGeometry = ChildDotWidget->DotImage->GetCachedGeometry();
        FVector2D ChildDotPosition = InGeometry.AbsoluteToLocal(ChildDotGeometry.GetAbsolutePosition());
        FVector2D ChildDotSize = ChildDotGeometry.GetLocalSize();

        // Perhitungan batas Child Dot
        FVector2D ChildLeftTop = ChildDotPosition - (ChildDotSize * 0.5f);
        FVector2D ChildRightBottom = ChildDotPosition + (ChildDotSize * 0.5f);

        // Periksa apakah kursor berada di dalam Child Dot
        if (CursorPosition.X >= ChildLeftTop.X && CursorPosition.X <= ChildRightBottom.X &&
            CursorPosition.Y >= ChildLeftTop.Y && CursorPosition.Y <= ChildRightBottom.Y)
        {
            UE_LOG(LogTemp, Warning, TEXT("Current i: %d"), i);
            UE_LOG(LogTemp, Warning, TEXT("LastChildDotIndex: %d"), LastChildDotIndex);

            // Jika urutan benar
            if (i != LastChildDotIndex && i == 0)
            {
                UE_LOG(LogTemp, Warning, TEXT("Cannot skip Child Dot %d! Resetting progress."), i);
                ResetProgress();
                return FReply::Unhandled();
            }

            // Update Temporary Line jika nilai berubah
            FGeometry ParentDotGeometry = GetDotParentWidgetFromIndex(ChildDotWidget->ParentDotIndex)->DotImage->GetCachedGeometry();
            FVector2D ParentDotPosition = InGeometry.AbsoluteToLocal(ParentDotGeometry.GetAbsolutePosition());

            // **Cek apakah TemporaryLinePoints sama**
            if (TemporaryLinePoints.Num() != 2 || TemporaryLinePoints[0] != ParentDotPosition || TemporaryLinePoints[1] != ChildDotPosition)
            {
                TemporaryLinePoints.Empty();
                TemporaryLinePoints.Add(ParentDotPosition);
                TemporaryLinePoints.Add(ChildDotPosition);

                UE_LOG(LogTemp, Warning, TEXT("Drawing Temporary Line:"));
                for (const FVector2D& TempPoint : TemporaryLinePoints)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Temp Point: %s"), *TempPoint.ToString());
                }
            }

            // Update indeks
            LastChildDotIndex = i;
            ProgressPercentage = FMath::CeilToFloat((float)(LastChildDotIndex + 1) / 11.0f);
            UE_LOG(LogTemp, Warning, TEXT("Updated LastChildDotIndex: %d, Percentage: %.1f%%"), LastChildDotIndex, ProgressPercentage);

            return FReply::Handled();
        }
    }

    return FReply::Handled();
}
*/

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
    /*
*/