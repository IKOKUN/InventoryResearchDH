// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent/InventoryComponent.h"
#include "../Data/InventoryItem.h"
#include "InventoryManagerComponent.generated.h"

class UInventoryComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYRESEARCHDH_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryManagerComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool TryAddItemToInventory(UInventoryComponent* Inventory, FInventoryItem& Item);
	void UseContainer(AActor* ContainerActor);
		
};
