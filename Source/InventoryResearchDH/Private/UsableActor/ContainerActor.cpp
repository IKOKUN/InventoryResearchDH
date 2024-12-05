// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor/ContainerActor.h"
#include "InventoryComponent/InventoryComponent.h"
#include "Controller/IRPlayerController.h"
#include "InventoryComponent/InventoryManagerComponent.h"

AContainerActor::AContainerActor()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	ActionText = FText::FromString("Open");
	/*InventoryComponent->SetCapacity(20);
	InventoryComponent->SetWeightCapacity(100);
	InventoryComponent->SetWeight(0);
	InventoryComponent->SetCapacity(0);
	InventoryComponent->SetOwner(this);*/
}

void AContainerActor::BeginPlay()
{
	Super::BeginPlay();

	InitInventory();
}

bool AContainerActor::OnActorUsed(APlayerController* PlayerController)
{
	if (bIsUsable)
	{
		AIRPlayerController* PC = Cast<AIRPlayerController>(PlayerController);
		if (PC)
		{
			PC->GetInventoryManagerComponent()->UseContainer(this);
			bool bSucces = Super::OnActorUsed(PlayerController);
			return bSucces;
		} else {
			return false;
		}	
	}
	else
	{
		return false;
	}
}

bool AContainerActor::InitInventory()
{
	InventoryComponent->InitInventory(InventorySize);
	return true;
}

bool AContainerActor::LoadInventoryItems(int32 InvSize, TArray<FInventoryItem> InvItems)
{
	InventorySize = InvSize;
	if (InventoryComponent)
	{
		bool bSucces = InventoryComponent->LoadInventoryItems(InventorySize, InvItems);

		for (FInventoryItem InvItem : InvItems)
		{
			UE_LOG(LogTemp, Log, TEXT("LoadInventoryItems On Container Actor: Item ID: %s, Amount: %d"), *InvItem.ID.ToString(), InvItem.Amount);
		}
		return bSucces;
	}
	return false;
}

FContainerProperties AContainerActor::GetContainerProperties()
{
	FContainerProperties ContainerProperties;
	ContainerProperties.ContainerName = Name;
	ContainerProperties.SlotsPerRow = SlotsPerRow;
	ContainerProperties.bIsStorageContainer = bCanStoreItem;
	ContainerProperties.InvetorySize = InventorySize;

	return ContainerProperties;
}

UInventoryComponent* AContainerActor::GetContainerInvetory()
{
	return InventoryComponent;
}

bool AContainerActor::GetCanStoreItems()
{
	return bCanStoreItem;
}

bool AContainerActor::ContainerLooted()
{
	bIsUsable = false;
	if (AIRPlayerController* IRPC = Cast<AIRPlayerController>(GetOwner()->GetInstigatorController()))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerController Invalid From Container Actor"));
		return false;
	}
}
