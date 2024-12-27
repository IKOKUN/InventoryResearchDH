// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Drawing/DotDrawWidget.h"
#include "Components/TextBlock.h"

void UDotDrawWidget::SetSequenceText(int32 Value) const
{
	if (SequenceTextBlock)
	{
		SequenceTextBlock->SetText(FText::FromString(FString::FromInt(Value)));
	}
}
