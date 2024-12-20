// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionActorBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UPrimitiveComponent;
UCLASS()
class INVENTORYRESEARCHDH_API AMissionActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissionActorBase();
	virtual void Tick(float DeltaTime) override;

	// Static Mesh Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Usable Actor")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Usable Actor")
	TObjectPtr<USceneComponent> SceneComponent;

	// Collision Component for the Actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Usable Actor")
	TObjectPtr<UBoxComponent> CollisionComponent;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	

public:	
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent.Get(); }
	FORCEINLINE USceneComponent* GetSceneComponent() const { return SceneComponent.Get(); }
	FORCEINLINE UBoxComponent* GetCollisionComponent() const { return CollisionComponent.Get(); }

};
