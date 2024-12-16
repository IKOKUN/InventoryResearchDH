// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/IRCharacter.h"
#include "IREquipmentCharacter.generated.h"

class USceneCaptureComponent2D;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API AIREquipmentCharacter : public AIRCharacter
{
	GENERATED_BODY()
	
public:
	AIREquipmentCharacter();
	

	
	// Scene Capture Component 2D
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;
protected:
	virtual void BeginPlay() override;

	TObjectPtr<AIRCharacter> PlayerCharacter;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateEquipmentMesh(bool bIsEquip) override;
	FORCEINLINE void SetPlayerCharacter(AIRCharacter* Player) { PlayerCharacter = Player; }
};
