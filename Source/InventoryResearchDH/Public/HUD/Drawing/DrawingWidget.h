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
	

	TArray<UDotDrawWidget*> DotParentWidgets;
	TArray<UDotDrawWidget*> DotChildWidgets;

    mutable TArray<FVector2D> LinePoints;
    TArray<FVector2D> TemporaryLinePoints;
    bool bIsDrawing = false; // Status menggambar garis
    int32 LastParentDotIndex; // Indeks dot terakhir yang disambungkan
	int32 LastChildDotIndex; // Indeks dot terakhir yang disambungkan

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drawing")
    FVector2D DotSize = FVector2D(50.f, 50.f);
    mutable bool bProgressCompleted = false;

protected:
	virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;

    float GetDPIScale();
    FVector2D GetViewportLocalPosition(FVector2D ScreenPosition);

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
private:
	float CurrentDeltaTime = 0.f;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBorder> CanvasBorder;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> DrawCanvasPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDotDrawWidget> DotParent0;

    UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDotDrawWidget> DotParent1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDotDrawWidget> DotParent2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDotDrawWidget> DotParent3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDotDrawWidget> DotParent4;

    UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDotDrawWidget> DotParent5;

    UPROPERTY(EditAnywhere, Category = "Dot Widget")
    int32 DotCount = 5;

    UPROPERTY(EditAnywhere, Category = "Dot Widget")
    TSubclassOf<UDotDrawWidget> DotChildWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Drawing")
	USoundBase* ConnectionSound;

	UPROPERTY(EditAnywhere, Category = "Drawing")
	USoundBase* CompletionSound;

	UPROPERTY(EditAnywhere, Category = "Drawing")
    mutable  double StartTime = 0;

	UPROPERTY(EditAnywhere, Category = "Drawing")
    double Duration = 1;

	float ProgressPercentage = 0.f;

    // Fungsi untuk membuat DotChild
	void CreatesAllDotChild();
	void ResetProgress();
	UDotDrawWidget* GetDotParentWidgetFromIndex(int32 Index);
    void UpdateTemporaryLinePoints();

    // Fungsi untuk menghitung percentage prgress berdasarkan lokasi mouse
    float GetCursorProjectionOnLine(const FVector2D& LineStart, const FVector2D& LineEnd, const FVector2D& CursorLoc, float Tolerance);
    void SpawnRandomDots(int32 Count);
	void PlayConnectionSound();
	void PlayCompletionSound();
};
