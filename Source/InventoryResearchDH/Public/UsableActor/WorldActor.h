// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor/UsableActorBase.h"
#include "Data/InventoryItem.h"
#include "WorldActor.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API AWorldActor : public AUsableActorBase
{
	GENERATED_BODY()
	
public:
	AWorldActor();

	bool LoadItemFromList();
	void UpdateItemAmount();
protected:
	virtual void BeginPlay() override;

	// IUsableActorInterface
	virtual bool OnActorUsed(APlayerController* PlayerController) override;
private:
	UPROPERTY(EditAnywhere, Category = "Item List")
	TObjectPtr<UDataTable> ItemList;

	UPROPERTY(EditAnywhere, Category = "Item List")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item List")
	int32 Amount;

	UPROPERTY(EditAnywhere, Category = "Item List")
	FInventoryItem InventoryItem;
};
