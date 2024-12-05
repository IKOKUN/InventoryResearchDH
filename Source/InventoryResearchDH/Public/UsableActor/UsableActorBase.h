// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/UsableActorInterface.h"
#include "Data/InventoryItem.h"
#include "Data/LootList.h"
#include "UsableActorBase.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable Actor")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	// Name of the Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FName Name = FName(TEXT("Null"));

	// Action of the Actor, Example : Use, Open, Pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FText ActionText = FText::FromString("Use");

	// Sound when Actor is used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	TObjectPtr<USoundBase> UsedSound;

	// Is Actor Usable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool bIsUsable = true;

	// Was Actor Used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool bWasUsed = false;

	// Get Inspect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
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
	AIRPlayerController* InteractorPlayerController = nullptr;
	AIRCharacter* InteractorPlayerCharacter = nullptr;
	APlayerCameraManager* IRCameraManager = nullptr;

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

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void InspectItem();

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void WaitForCamera();

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void StartInspection();

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void CameraFocus();

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void EndInspection();

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void RotateObjectX(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void RotateObjectY(float AxisValue);
	// End Inspect Object

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> InspectWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<UInspectWidget> CurrentInspectWidget;

protected:
	virtual void BeginPlay() override;

	// Fungsi untuk mendapatkan data berdasarkan RowName
	bool GetDataTableRowByName(UDataTable* SrcDataTable, const FName RowName, FInventoryItem& OutInvItemRow);
	bool GetDataTableRowByName(UDataTable* SrcDataTable, const FName RowName, FLootList& OutLootListRow);

	UPROPERTY(EditAnywhere, Category = "Data Table")
	TObjectPtr<UDataTable> DataTable;

public:
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }
};
