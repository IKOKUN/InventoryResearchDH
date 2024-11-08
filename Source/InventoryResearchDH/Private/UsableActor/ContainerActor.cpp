// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor/ContainerActor.h"
#include "InventoryComponent/InventoryComponent.h"

AContainerActor::AContainerActor()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
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

void AContainerActor::InitInventory()
{
	InventoryComponent->InitInventory();
}

void AContainerActor::LoadInventoryItems()
{
	//InventoryComponent->LoadInventoryItems();
}

FContainerProperties AContainerActor::GetContainerProperties()
{
	return FContainerProperties();
}

UInventoryComponent* AContainerActor::GetContainerInvetory()
{
	return nullptr;
}

bool AContainerActor::GetCanStoreItems()
{
	return false;
}

bool AContainerActor::ContainerLooted()
{
	return false;
}
