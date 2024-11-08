// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IRHUD.generated.h"

class UUSerWidget;
/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API AIRHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AIRHUD();

    // Override fungsi ReceiveDrawHUD
    virtual void DrawHUD() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    TSubclassOf<UUserWidget> MainHUDWidgetClass;

    // Pointer ke instance widget yang akan diinisialisasi
    UPROPERTY()
    TObjectPtr<UUserWidget> MainHUDWidgetInstance;
protected:
	virtual void BeginPlay() override;

    // Fungsi khusus yang akan menggambar elemen-elemen HUD
    void DrawCrosshair();
    void DrawPlayerStats();

	
};
