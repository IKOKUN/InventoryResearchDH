// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Drawing/DotDrawWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDotDrawWidget::SetSequenceText(int32 Value) const
{
	if (SequenceTextBlock)
	{
		SequenceTextBlock->SetText(FText::FromString(FString::FromInt(Value)));
	}
}

void UDotDrawWidget::SetDotImageBrushColorToGreen()
{
	if (DotImage)
	{
        DotImage->SetColorAndOpacity(FLinearColor::Green);
	}
}

void UDotDrawWidget::SetDotImageTexture(bool bComplete)
{
	if (DotImage)
	{
		if (bComplete && CompleteDotTexture)
		{
			DotImage->SetBrushFromTexture(CompleteDotTexture);
		}
		else if (ConnectionDotTexture)
		{
			DotImage->SetBrushFromTexture(ConnectionDotTexture);
		}
	}
}

void UDotDrawWidget::PlayDotAnimation()
{
	if (ConnectionDotAnimation)
	{
		PlayAnimation(ConnectionDotAnimation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ConnectionDotAnimation is null"));
	}
}

FVector2D UDotDrawWidget::GetDotCenterPosition() const
{
	return FVector2D();
}
