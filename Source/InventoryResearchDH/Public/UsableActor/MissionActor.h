// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor/UsableActorBase.h"
#include "MissionActor.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API AMissionActor : public AUsableActorBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	FName RequiredItemID;

	// IUsableActorInterface
	virtual bool OnActorUsed(APlayerController* PlayerController) override;
	virtual bool OnActorInspect(APlayerController* PlayerController) override;
	virtual bool BeginOutlineFocus() override;
	virtual bool EndOutlineFocus() override;
	virtual bool GetIsActorUsable() override;
	virtual FText GetUseActionText() override;
	// end IUsableActorInterface

public:
	FORCEINLINE FName GetRequiredItemID() const { return RequiredItemID; }
};
