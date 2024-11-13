// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IRPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API AIRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AIRPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

protected:
	/* Base Movement Input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJumping();
	/* End Base Movement Input */

	/* Player Inventory Component for Handling Player Inventory*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UEquipmentInventoryComponent> PlayerInventoryComponent;

	/* Inventory Manager Component for connect Player Controller with UI*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UInventoryManagerComponent> InventoryManagerComponent;
private:
	/* Input */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_InventoryResearch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	/* End Input */

	/* Character Properties */
	UPROPERTY(EditAnywhere, Category = "Character")
	FName Name = FName(TEXT("Player"));

	UPROPERTY(EditAnywhere, Category = "Character")
	float Health = 60.f;

	UPROPERTY(EditAnywhere, Category = "Character")
	float MaxHealth = 100.0f;
	/* Character Properties */

public:
	FORCEINLINE UEquipmentInventoryComponent* GetPlayerInventoryComponent() const { return PlayerInventoryComponent.Get(); }
	FORCEINLINE UInventoryManagerComponent* GetInventoryManagerComponent() const { return InventoryManagerComponent.Get(); }
	FORCEINLINE FName GetPlayerName() const { return Name; }
	FORCEINLINE float GetPlayerHealth() const { return Health; }
	FORCEINLINE float GetPlayerMaxHealth() const { return MaxHealth; }
};
