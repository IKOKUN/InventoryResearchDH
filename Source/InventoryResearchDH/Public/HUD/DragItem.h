// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "../Data/ItemInformation.h"
#include "DragItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API UDragItem : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop", meta = (ExposeOnSpawn = "true", DisplayName = "Dragged Item Information"))
	FItemInformation DraggedItemInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop", meta = (ExposeOnSpawn = "true", DisplayName = "Dragged Item Slot Index"))
	int32 DraggedItemSlotIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop", meta = (ExposeOnSpawn = "true", DisplayName = "Dragged Item Slot Type"))
	bool bIsDraggedFromInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop", meta = (ExposeOnSpawn = "true", DisplayName = "Dragged Item Slot Type"))
	bool bIsDraggedFromContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop", meta = (ExposeOnSpawn = "true", DisplayName = "Dragged Item Slot Type"))
	bool bIsDraggedFromHotbar;

};
