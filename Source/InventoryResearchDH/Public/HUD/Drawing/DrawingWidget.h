// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DrawingWidget.generated.h"

class UBorder;
class UCanvasPanel;
class UDotDrawWidget;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UDrawingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Dot Widget")
	int32 DotCount = 5;

	TArray<UDotDrawWidget*> DotWidgets;
    mutable TArray<FVector2D> LinePoints;
    TArray<FVector2D> TemporaryLinePoints;
    bool bIsDrawing = false; // Status menggambar garis
    int32 LastDotIndex; // Indeks dot terakhir yang disambungkan

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drawing")
    FVector2D DotSize = FVector2D(50.f, 50.f);
    mutable bool bProgressCompleted = false;

protected:
    virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;

    float GetDPIScale();
    FVector2D GetViewportLocalPosition(FVector2D ScreenPosition);

    bool IsPointInsideDot(const FVector2D& Point, const FVector2D& DotPosition) const;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    void HandleMouseUpOnDot(int32 DotIndex, const FVector2D& DotPosition);
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    void UpdateTemporaryLine(const FVector2D& CursorPosition);


    virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
private:
	float CurrentDeltaTime = 0.f;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBorder> CanvasBorder;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> DrawCanvasPanel;

    UPROPERTY(EditAnywhere, Category = "Dot Widget")
    TSubclassOf<UDotDrawWidget> DotDrawWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Drawing")
	USoundBase* ConnectionSound;

	UPROPERTY(EditAnywhere, Category = "Drawing")
	USoundBase* CompletionSound;

	UPROPERTY(EditAnywhere, Category = "Drawing")
    mutable  double StartTime = 0;

	UPROPERTY(EditAnywhere, Category = "Drawing")
    double Duration = 1;



    void ResetDrawingState();

    void SpawnRandomDots(int32 Count);
	void PlayConnectionSound();
	void PlayCompletionSound();
    bool GenerateValidDotPosition(const FVector2D& BorderSize, const TArray<FVector2D>& ExistingPositions, FVector2D& OutPosition) const;
    void AddDotToCanvas(UDotDrawWidget* DotWidget, const FVector2D& Position);
};
