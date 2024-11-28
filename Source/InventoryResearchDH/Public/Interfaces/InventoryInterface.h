// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

class UInventoryComponent;

USTRUCT(BlueprintType)
struct FContainerProperties
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FName ContainerName = FName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	uint8 SlotsPerRow = uint8();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool bIsStorageContainer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 InvetorySize = 0;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYRESEARCHDH_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FContainerProperties GetContainerProperties();
	virtual UInventoryComponent* GetContainerInvetory();
	virtual bool GetCanStoreItems();
	virtual bool ContainerLooted();
};
