// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InventoryInterface.h"

// Add default functionality here for any IInventoryInterface functions that are not pure virtual.

FContainerProperties IInventoryInterface::GetContainerProperties()
{
    return FContainerProperties();
}

UInventoryComponent* IInventoryInterface::GetContainerInvetory()
{
    return nullptr;
}

bool IInventoryInterface::GetCanStoreItems()
{
    return false;
}

bool IInventoryInterface::ContainerLooted()
{
    return false;
}
