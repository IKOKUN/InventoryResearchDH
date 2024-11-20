// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent/EquipmentInventoryComponent.h"
#include "Character/IRCharacter.h"
#include "Data/EquipmentSlots.h"

UEquipmentInventoryComponent::UEquipmentInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UEquipmentInventoryComponent::InitInventory(int32 InventorySize)
{
	InventoryItems.Empty();
	for (int32 i = 0; i < GetEnumEquimentSlotsElementCount() + (InventorySize - 1); i++)
	{
		InventoryItems.Add(FInventoryItem());
	}
	return true;
}

bool UEquipmentInventoryComponent::SetInventoryItem(int32 InvSlot, FInventoryItem Item)
{
	bool bSucces = Super::SetInventoryItem(InvSlot, Item);

	UpdateEquippedMeshes(InvSlot);
	return bSucces;
}

bool UEquipmentInventoryComponent::ClearInventoryItem(int32 InvSlotIndex)
{
	bool bSucces = Super::ClearInventoryItem(InvSlotIndex);

	UpdateEquippedMeshes(InvSlotIndex);
	return bSucces;
}

bool UEquipmentInventoryComponent::GetEmptyInventorySpace(int32& OutIndex)
{
	int32 LocalIndex = 0;

	// Iterasi melalui setiap item di inventory
	for (FInventoryItem LocInvItem : InventoryItems)
	{
		// Jika icon kosong (slot kosong)
		if (!LocInvItem.Icon)
		{
			// Jika LocalIndex lebih besar atau sama dengan jumlah slot yang diinginkan
			if (LocalIndex >= GetEnumEquimentSlotsElementCount())
			{
				// Setel indeks keluar dan kembalikan true
				OutIndex = LocalIndex;
				return true; // Menghentikan loop setelah menemukan slot kosong
			}
		}
		LocalIndex++;
	}

	// Jika tidak ditemukan slot kosong, kembalikan false
	return false;
}


bool UEquipmentInventoryComponent::LoadInventoryItems(int32 InventorySize, TArray<FInventoryItem> InvItems)
{
	int32 TempIndex = InventorySize + GetEnumEquimentSlotsElementCount();
	bool bSucces = Super::LoadInventoryItems(TempIndex, InvItems);
	return bSucces;
}

void UEquipmentInventoryComponent::UpdateEquippedMeshes(int32 InvSlot)
{
	USkeletalMesh* LocalEquipmentMesh = nullptr;
	TSubclassOf<AActor> LocalWeaponActorClass = nullptr;
	if (PlayerCharacter)
	{
		if (InvSlot < GetEnumEquimentSlotsElementCount())
		{
			FInventoryItem LocalInvItem = GetInventoryItem(InvSlot);
			LocalWeaponActorClass = LocalInvItem.WeaponActorClass;
			LocalEquipmentMesh = LocalInvItem.EquipmentMesh;

			EEquipmentSlots LocEquipmentSlot = static_cast<EEquipmentSlots>(InvSlot);
			switch (LocEquipmentSlot)
			{
			case EEquipmentSlots::Head:
				PlayerCharacter->HeadMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::Shoulder:
				PlayerCharacter->ShoulderMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::Chest:
				PlayerCharacter->ChestMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::Hands:
				PlayerCharacter->HandsMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::Legs:
				PlayerCharacter->LegsMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::Feet:
				PlayerCharacter->FeetMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::Accessory:
				PlayerCharacter->AccessoryMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::Back:
				PlayerCharacter->BackMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::RightRing:
				PlayerCharacter->RightRingMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::LeftRing:
				PlayerCharacter->LeftRingMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::Waist:
				PlayerCharacter->WaistMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::Trinket:
				//PlayerCharacter->HeadMesh = LocalEquipmentMesh;
				break;
			case EEquipmentSlots::MainHand:
				UpdateMainHandWeapon(LocalWeaponActorClass, LocalEquipmentMesh);
				break;
			case EEquipmentSlots::OffHand:
				PlayerCharacter->OffHandMesh = LocalEquipmentMesh;
				break;
			default:
				break;
			}
		}
	}
}

void UEquipmentInventoryComponent::UpdateMainHandWeapon(TSubclassOf<AActor> WeaponActorClass, USkeletalMesh* EquipmentMesh)
{
	if (PlayerCharacter->MainHandWeaponRef)
	{
		PlayerCharacter->MainHandWeaponRef->Destroy();
		PlayerCharacter->MainHandWeaponRef = nullptr;
		if (WeaponActorClass && GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = PlayerCharacter;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
				WeaponActorClass,
				PlayerCharacter->GetActorTransform(),
				SpawnParams);
			PlayerCharacter->MainHandWeaponRef = SpawnedActor;
			FAttachmentTransformRules SpawnedAttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
			SpawnedActor->AttachToComponent(PlayerCharacter->GetMesh(), SpawnedAttachmentRules, FName("MainHand"));
		}
		else
		{
			PlayerCharacter->MainHandWeaponRef = nullptr;
		}
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
}

void UEquipmentInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

int32 UEquipmentInventoryComponent::GetEnumEquimentSlotsElementCount()
{
	UEnum* Enum = StaticEnum<EEquipmentSlots>();
	int32 LocEquipmentSlots = 0;
	if (Enum)
	{
		UE_LOG(LogTemp, Log, TEXT("Jumlah elemen dalam enum: %d"), Enum->NumEnums() - 1);
		LocEquipmentSlots = Enum->NumEnums() - 1; // Kurangi 1 untuk mengabaikan "_MAX" atau elemen tersembunyi		
	}
	return LocEquipmentSlots;
}

void UEquipmentInventoryComponent::SetPlayerCharacter(AIRCharacter* Character)
{
	PlayerCharacter = Character;
}
