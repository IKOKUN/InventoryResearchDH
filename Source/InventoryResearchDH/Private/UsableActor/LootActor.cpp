// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor/LootActor.h"

ALootActor::ALootActor()
{
	SlotsPerRow = 1;
	bCanStoreItem = false;
	ActionText = FText::FromString("Loot");
}

void ALootActor::BeginPlay()
{
}
