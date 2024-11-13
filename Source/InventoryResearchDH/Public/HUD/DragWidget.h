// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragWidget.generated.h"

class WidgetToDrag;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UDragWidget : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop", meta = (ExposeOnSpawn = "true", DisplayName = "Drop Window Offset"))
	FVector2D DropWindowOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drag and Drop", meta = (ExposeOnSpawn = "true", DisplayName = "WidgetToDrag"))
	TObjectPtr<UUserWidget> WidgetToDrag;
};
