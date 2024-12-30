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
    TArray<FVector2D> LinePoints;
    TArray<FVector2D> TemporaryLinePoints;
    bool bIsDrawing = false; // Status menggambar garis
    int32 LastDotIndex; // Indeks dot terakhir yang disambungkan

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drawing")
    FVector2D DotSize = FVector2D(50.f, 50.f);

protected:
    virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


    virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBorder> CanvasBorder;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> DrawCanvasPanel;

    UPROPERTY(EditAnywhere, Category = "Dot Widget")
    TSubclassOf<UDotDrawWidget> DotDrawWidgetClass;

    void SpawnRandomDots(int32 Count);
};
