// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionActor/MissionActorBase.h"

#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Character/IRCharacter.h"

AMissionActorBase::AMissionActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMissionActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMissionActorBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AIRCharacter* InteractCharacter = Cast<AIRCharacter>(OtherActor);
	if (InteractCharacter)
	{

	}
}

void AMissionActorBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMissionActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

