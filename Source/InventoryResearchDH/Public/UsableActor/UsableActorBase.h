// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/UsableActorInterface.h"
#include "Data/InventoryItem.h"
#include "Data/LootList.h"
#include "UsableActorBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USoundBase;
class UDataTable;
class AIRPlayerController;
class AIRCharacter;
class APlayerCameraManager;
class UInspectWidget;

UCLASS()
class INVENTORYRESEARCHDH_API AUsableActorBase : public AActor, public IUsableActorInterface
{
	GENERATED_BODY()
	
public:	
	AUsableActorBase();

	// function when Actor is used
	bool OnWasUsed();

	// Static Mesh Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Usable Actor")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Usable Actor")
	TObjectPtr<USceneComponent> SceneComponent;

	// Name of the Actor
	UPROPERTY(EditAnywhere, Category = "Properties")
	FName Name = FName(TEXT("Null"));

	// Action of the Actor, Example : Use, Open, Pickup
	UPROPERTY(EditAnywhere, Category = "Properties")
	FText ActionText = FText::FromString("Use");

	// Sound when Actor is used
	UPROPERTY(EditAnywhere, Category = "Properties")
	TObjectPtr<USoundBase> UsedSound;

	// Can Interact with Actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	bool bCanInspect = true;

	// Interact 3D Or 2D
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	bool bInteract3D = true;

	bool bIsUsable = true;
	bool bWasUsed = false;
	bool bGettingInspect = false;


	// IUsableActorInterface
	virtual bool OnActorUsed(APlayerController* PlayerController) override;
	virtual bool OnActorInspect(APlayerController* PlayerController) override;
	virtual bool BeginOutlineFocus() override;
	virtual bool EndOutlineFocus() override;
	virtual bool GetIsActorUsable() override;
	virtual FText GetUseActionText() override;
	// end IUsableActorInterface

	// Inspect Object
	TObjectPtr<AIRPlayerController> InteractorPlayerController = nullptr;
	TObjectPtr<AIRCharacter> InteractorPlayerCharacter = nullptr;
	TObjectPtr<APlayerCameraManager> IRCameraManager = nullptr;

	FTimerHandle TimerHandle_WaitForCamera;

	FTransform CameraTransformCache;
	FVector OriginRelativeLocation;
	FVector ObjectOriginLocation;
	FVector InspectionLocation;

	FRotator ControlRotationOrigin;
	FRotator TargetControlRotation;
	FRotator ObjectOriginRotation;
	FRotator NewObjectRotation;

	float InspectionDistance = 120.f;
	float HoldFocusTime = -1.f;
	float RotationFactorX;
	float RotationFactorY;

	int32 RotateYQuarter = 0;

	// Inspect Object Function
	void InspectItem();
	void WaitForCamera();
	void StartInspection();
	void CameraFocus();
	void EndInspection();
	void RotateObjectX(float AxisValue);
	void RotateObjectY(float AxisValue);
	// End Inspect Object

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> InspectWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<UInspectWidget> CurrentInspectWidget;

protected:
	virtual void BeginPlay() override;

	// Fungsi untuk mendapatkan data berdasarkan RowName
	bool GetDataTableRowByName(TSoftObjectPtr<UDataTable> SrcDataTableSoftRef, const FName RowName, FInventoryItem& OutInvItemRow);
	bool GetDataTableRowByName(TSoftObjectPtr<UDataTable> SrcDataTableSoftRef, const FName RowName, FLootList& OutLootListRow);

	UPROPERTY(EditAnywhere, Category = "Data Table")
	TSoftObjectPtr<UDataTable> DataTable;

public:
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }
};
