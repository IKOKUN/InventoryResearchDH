// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent/InventoryComponent.h"
#include "../Data/InventoryItem.h"
#include "InventoryManagerComponent.generated.h"

class UInventoryComponent;
class UUserWidget;
class UInventoryLayoutWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYRESEARCHDH_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool TryAddItemToInventory(UInventoryComponent* Inventory, FInventoryItem& Item);
	void UseContainer(AActor* ContainerActor);

	void InitPlayerInventory(int32 InventorySize);
	void InitInventoryManager(UInventoryComponent* PlayerInventoryRef);
	void InitInventoryItems();

	/* Function User Interface */
	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void InitInventoryManagerUI(UInventoryLayoutWidget* InventoryWidget);


	/* Custom Event */
	UFUNCTION(BlueprintCallable, Category = "Custom Event")
	void LoadInventory();
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<FInventoryItem> Items;

	/* Components */
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInventoryComponent> PlayerInventory;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInventoryComponent> ContainerInventory;
	/* End Components */

	/* User Interface*/
	UPROPERTY(BlueprintReadOnly, Category = "User Interface")
	bool bIsInventoryOpen;

	UPROPERTY(BlueprintReadOnly, Category = "User Interface")
	bool bIsContainerOpen;

	UPROPERTY(BlueprintReadOnly, Category = "User Interface")
	bool bIsEquipmentOpen;

	UPROPERTY(BlueprintReadOnly, Category = "User Interface")
	uint8 InventorySlotsPerRow = 6;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "User Interface")
	TObjectPtr<UInventoryLayoutWidget> InventoryUI;
	/* End User Interface*/
public:	
	FORCEINLINE bool IsInventoryOpen() const { return bIsInventoryOpen; }
	FORCEINLINE bool IsContainerOpen() const { return bIsContainerOpen; }
	FORCEINLINE bool IsEquipmentOpen() const { return bIsEquipmentOpen; }
	FORCEINLINE UINT8 GetInventorySlotsPerRow() const { return InventorySlotsPerRow; }
	FORCEINLINE UInventoryComponent* GetPlayerInventory() const { return PlayerInventory; }
	FORCEINLINE UInventoryComponent* GetContainerInventory() const { return ContainerInventory; }
		
};
