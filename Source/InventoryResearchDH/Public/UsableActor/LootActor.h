// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor/ContainerActor.h"
#include "LootActor.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYRESEARCHDH_API ALootActor : public AContainerActor
{
	GENERATED_BODY()
	
public:
	ALootActor();

protected:
	virtual void BeginPlay() override;
};
