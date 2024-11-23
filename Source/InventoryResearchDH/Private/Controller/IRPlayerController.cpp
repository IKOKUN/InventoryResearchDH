// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/IRPlayerController.h"

#include "Character/IRCharacter.h"
#include "HUD/IRHUD.h"
#include "HUD/HUDLayoutWidget.h"
#include "HUD/InteractTextWidget.h"
#include "InventoryComponent/EquipmentInventoryComponent.h"
#include "InventoryComponent/InventoryManagerComponent.h"
#include "UsableActor/UsableActorBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TimerManager.h"

AIRPlayerController::AIRPlayerController()
{
	bReplicates = true;

	PlayerInventoryComponent = CreateDefaultSubobject<UEquipmentInventoryComponent>(TEXT("PlayerInventoryComponent"));
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryManagerComponent"));
}

void AIRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializePlayer();
}

void AIRPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AIRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(IMC_InventoryResearch, 0);
		UE_LOG(LogTemp, Log, TEXT("Mapping context added successfully."));
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		UE_LOG(LogTemp, Log, TEXT("EnhancedInputComponent found and ready."));
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AIRPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AIRPlayerController::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AIRPlayerController::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AIRPlayerController::Look);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AIRPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
		//UE_LOG(LogTemp, Log, TEXT("Move function called"));
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("No controlled pawn found."));
	}
}

void AIRPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AIRPlayerController::Jump()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->Jump();
	}
}

void AIRPlayerController::StopJumping()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->StopJumping();
	}
}

bool AIRPlayerController::SetHealth(int32 Amount, float Duration)
{
	int32 LocalAmount = Amount;
	int32 LocalDuration = Duration;

	if (Health == MaxHealth && LocalAmount > 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
				5.0f,                 // Durasi pesan dalam detik
				FColor::Green,        // Warna pesan
				FString::Printf(TEXT("You Are Full")) // Pesan
			);
			
		}
		return false;
	}
	else
	{
		if (bIsPoisoned)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
					5.0f,                 // Durasi pesan dalam detik
					FColor::Green,        // Warna pesan
					FString::Printf(TEXT("You Are Too Sick To Eat")) // Pesan
				);

			}
			return false;
		}
		else if (LocalDuration > 0)
		{
			bIsPoisoned = LocalAmount < 0 ? true : false;
			int32 RegenLeft = RegenDuration - RegenTickCount;
			float AmountHealthLeftToRegen = RegenLeft * HealthTick;
			if (AmountHealthLeftToRegen > LocalAmount && !bIsPoisoned)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
						5.0f,                 // Durasi pesan dalam detik
						FColor::Green,        // Warna pesan
						FString::Printf(TEXT("You Have A Stronger Food Buff Active")) // Pesan
					);

				}
				return false;
			}
			else
			{
				HealthTick = LocalAmount / LocalDuration;
				RegenDuration = trunc(LocalDuration);
				RegenTickCount = 0;
				RegenHealth();
				return true;
			}
		}
		else
		{
			Health = FMath::Clamp(Health + LocalAmount, 0.0f, MaxHealth);
			return true;
		}
	}
}

void AIRPlayerController::RegenHealth()
{
	if (GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(RegenTimerHandle))
	{
		RegenTickCount = 0; // Reset tick count
		GetWorld()->GetTimerManager().SetTimer(
			RegenTimerHandle,
			this,
			&AIRPlayerController::StartRegenHealth,
			1.0f,
			true,
			0.f
		);
	}
}

void AIRPlayerController::StartRegenHealth()
{
	if (RegenTickCount >= RegenDuration)
	{
		StopRegenHealth();
	}
	else
	{
		Health = FMath::Clamp(Health + HealthTick, 0.0f, MaxHealth);
		RegenTickCount++;
	}
}

void AIRPlayerController::StopRegenHealth()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);

		if (bIsPoisoned)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,                   // Kunci pesan (-1 untuk pesan baru setiap kali)
					5.0f,                 // Durasi pesan dalam detik
					FColor::Green,        // Warna pesan
					FString::Printf(TEXT("You Are No Longer Poisoned")) // Pesan
				);
			}
		}
		bIsPoisoned = false;
	}
}

void AIRPlayerController::SetInteractText(FText InputText)
{
	if (HUDReference)
	{
		HUDReference->InteractText->MessageText = InputText;
	}
}

FText AIRPlayerController::GetActorActionText(AUsableActorBase* UsableActor)
{
	if (UsableActor)
	{
		return FText::FromString(FString::Printf(TEXT("%s %s"), *UsableActor->ActionText.ToString(), *UsableActor->Name.ToString()));

	}
	return FText();
}

void AIRPlayerController::ShowInteractText()
{
	if (HUDReference)
	{
		if (!HUDReference->InteractText->MessageText.IsEmpty())
		{
			HUDReference->InteractText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void AIRPlayerController::HideInteractText()
{
	if (HUDReference && HUDReference->InteractText)
	{
		HUDReference->InteractText->SetVisibility(ESlateVisibility::Hidden);
	}
}

AUsableActorBase* AIRPlayerController::GetUsableActor()
{
	if (!PlayerCharacter)
	{
		return nullptr;
	}

	// 1. Ambil lokasi dan rotasi dari FollowCamera
	UCameraComponent* FollowCamera = PlayerCharacter->GetFollowCamera();
	USpringArmComponent* CameraBoom = PlayerCharacter->GetCameraBoom();

	if (!FollowCamera || !CameraBoom)
	{
		UE_LOG(LogTemp, Warning, TEXT("FollowCamera or CameraBoom not found"));
		return nullptr;
	}

	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();

	// 2. Tentukan posisi End menggunakan ForwardVector dan MaxUseDistance
	FVector End = Start + (ForwardVector * (MaxUseDistance + CameraBoom->TargetArmLength));

	// 3. Konfigurasi Trace
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerCharacter); // Abaikan karakter pemain
	Params.bTraceComplex = false;      // Tidak menggunakan Trace Complex

	// 4. Lakukan LineTraceByChannel
	bool bHit = PlayerCharacter->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,  // Trace Channel (sama seperti Visibility di blueprint)
		Params
	);

	// 5. Debug: Gambar garis di dunia untuk memverifikasi
	DrawDebugLine(PlayerCharacter->GetWorld(), Start, End, FColor::Green, false, 2.0f);

	// 6. Jika Line Trace mengenai sesuatu
	if (bHit && HitResult.GetActor())
	{
		AUsableActorBase* UsableActor = Cast<AUsableActorBase>(HitResult.GetActor());
		if (UsableActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
			return UsableActor; // Mengembalikan Actor yang ditemukan
		}	
	}

	return nullptr;
}

void AIRPlayerController::OnActorUsed()
{
	if (GetUsableActor())
	{
		GetUsableActor()->OnActorUsed(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No UsableActor found"));
	}
}

void AIRPlayerController::GetUsableActorFocus()
{
	if (GetUsableActor())
	{
		if (GetUsableActor() != LastUsableActor)
		{
			bIsNewFocus = true;
			if (LastUsableActor)
			{
				LastUsableActor->EndOutlineFocus();
				LastUsableActor = nullptr;
				SetInteractText(FText());
				HideInteractText();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("LastUsableActor is not a UsableActor"));
			}
		}

		LastUsableActor = GetUsableActor();
		if (bIsNewFocus)
		{
			LastUsableActor->BeginOutlineFocus();
			bIsNewFocus = false;
			SetInteractText(GetActorActionText(LastUsableActor));
			ShowInteractText();
		}
	}
	else
	{
		SetInteractText(FText());
		HideInteractText();
	}

	if (LastUsableActor)
	{
		if (!LastUsableActor->GetIsActorUsable())
		{
			LastUsableActor->EndOutlineFocus();
			if (InventoryManagerComponent)
			{
				if (InventoryManagerComponent->GetCurrentContainer() == Cast<AContainerActor>(LastUsableActor))
				{
					InventoryManagerComponent->CloseContainerWindow();
				}
			}
			SetInteractText(FText());
			HideInteractText();

		}	
	}
}

void AIRPlayerController::InitializePlayer()
{
	// Cast the controlled pawn to AIRCharacter
	PlayerCharacter = Cast<AIRCharacter>(GetPawn());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is not valid"));
		return;
	}

	// Initialize Player Inventory Component
	if (PlayerInventoryComponent)
	{
		PlayerInventoryComponent->SetPlayerCharacter(PlayerCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInventoryComponent is not valid"));
		return;
	}

	// Initialize Inventory Manager Component
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->InitInventoryManager(PlayerInventoryComponent);
		LoadPlayerItems();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
		return;
	}

	// Get HUD and validate it
	AIRHUD* MainHUD = Cast<AIRHUD>(GetHUD());
	if (!MainHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD not found or is invalid"));
		return;
	}

	// Assign the MainHUD widget reference
	HUDReference = MainHUD->MainHUDWidgetInstance;
	if (!HUDReference)
	{
		UE_LOG(LogTemp, Error, TEXT("HUDReference (MainHUDWidgetInstance) is not valid"));
		return;
	}

	// Check if this is a local player controller
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Configure input mode and mouse cursor
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);

	if (HUDReference->IsValidLowLevel())
	{
		InputModeData.SetWidgetToFocus(HUDReference->TakeWidget());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HUDReference is invalid for setting WidgetToFocus"));
		return;
	}

	SetInputMode(InputModeData);

	// Log the controlled pawn
	APawn* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn)
	{
		UE_LOG(LogTemp, Log, TEXT("Pawn controlled by this PlayerController: %s"), *ControlledPawn->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Pawn is being controlled by this PlayerController"));
	}

	// Initialize Inventory UI and load inventory
	if (HUDReference->InventoryLayout && InventoryManagerComponent)
	{
		InventoryManagerComponent->InitInventoryManagerUI(HUDReference->InventoryLayout);
		InventoryManagerComponent->LoadInventory();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HUDReference->InventoryLayout or InventoryManagerComponent is not valid"));
	}

	// Unlock movement
	bIsMovementLocked = false;
}

// Fungsi untuk mengonversi enum ke FName
FName AIRPlayerController::EnumToFNameByValue(EClassesHero EnumValue)
{
	UEnum* EnumPtr = StaticEnum<EClassesHero>();
	if (!EnumPtr) return FName("Invalid");

	return EnumPtr->GetNameByValue((int64)EnumValue);
}

// Atau menggunakan FName berdasarkan nilai enum langsung
FName AIRPlayerController::EnumToFName(EClassesHero EnumValue)
{
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EClassesHero"), true);
	if (!EnumPtr) return FName("Invalid");

	// Mengonversi enum ke FName berdasarkan index
    return FName(*EnumPtr->GetNameStringByIndex((int32)EnumValue));
}

void AIRPlayerController::LoadPlayerItems()
{
	TArray<FName> LocalItemIds;
	TArray<FInventoryItem> LocalInventoryItems;
	FName LocalTestID = FName("Tandy");
	EClassesHero LocalHeroClass = EClassesHero::Warrior;
	FNPCItems LocalNPCItems;

	int32 RandomInteger = FMath::RandRange(0, 3 - 1);
	LocalHeroClass = static_cast<EClassesHero>(RandomInteger);

	if (GetDataTableRowByName(NPCItemsDataTable, EnumToFNameByValue(LocalHeroClass), LocalNPCItems))
	{
		LocalItemIds.Add(LocalNPCItems.Head);
		LocalItemIds.Add(LocalNPCItems.Chest);
		LocalItemIds.Add(LocalNPCItems.Legs);
		LocalItemIds.Add(LocalNPCItems.Feet);
		LocalItemIds.Add(LocalNPCItems.Accessory);
		LocalItemIds.Add(LocalNPCItems.Back);
		LocalItemIds.Add(LocalNPCItems.RightRing);
		LocalItemIds.Add(LocalNPCItems.LeftRing);
		LocalItemIds.Add(LocalNPCItems.Waist);
		LocalItemIds.Add(LocalNPCItems.Trinket);
		LocalItemIds.Add(LocalNPCItems.MainHand);
		LocalItemIds.Add(LocalNPCItems.OffHand);
		LocalItemIds.Add(LocalNPCItems.Shoulders);
		LocalItemIds.Add(LocalNPCItems.Hands);

		FInventoryItem LocalInventoryItem1;
		int32 LocalIndex = 0;
		for (FName itmid : LocalItemIds)
		{
			if (GetDataTableRowByName(ItemListDataTable, itmid, LocalInventoryItem1))
			{
				SetInventoryArrayElement(LocalInventoryItems, LocalIndex, LocalInventoryItem1, true);
			}
			else
			{
				SetInventoryArrayElement(LocalInventoryItems, LocalIndex, FInventoryItem(), true);
			}

			LocalIndex++;
		}
		FInventoryItem LocalInventoryItem2;
		for (FItem LocItem : LocalNPCItems.Inventory)
		{
			if (GetDataTableRowByName(ItemListDataTable, LocItem.ItemID, LocalInventoryItem2))
			{
				LocalInventoryItem2.Amount = LocItem.Amount;
				if (LocalInventoryItem2.ItemType == EItemType::Currency)
				{
					InventoryManagerComponent->TryAddItemToInventory(PlayerInventoryComponent, LocalInventoryItem2);
				}
				else
				{
					LocalInventoryItems.Add(LocalInventoryItem2);
				}
			}
		}
		PlayerInventoryComponent->LoadInventoryItems(LocalInventoryItems.Num(), LocalInventoryItems);
	}
	else
	{
		InventoryManagerComponent->InitInventoryItems();
	}
    
}

bool AIRPlayerController::GetDataTableRowByName(UDataTable* SrcDataTable, const FName RowName, FNPCItems& OutNPCInvItemRow)
{
	if (!SrcDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
		return false;
	}

	FNPCItems* Row = SrcDataTable->FindRow<FNPCItems>(RowName, TEXT(""));

	if (Row)
	{
		OutNPCInvItemRow = *Row; // Ensure FNPCItems has a valid assignment operator
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Row not found: %s"), *RowName.ToString());
		return false;
	}
}

bool AIRPlayerController::GetDataTableRowByName(UDataTable* SrcDataTable, const FName RowName, FInventoryItem& OutNPCInvItemRow)
{
	if (!SrcDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
		return false;
	}

	FInventoryItem* Row = SrcDataTable->FindRow<FInventoryItem>(RowName, TEXT(""));

	if (Row)
	{
		OutNPCInvItemRow = *Row; // Ensure FNPCItems has a valid assignment operator
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Row not found: %s"), *RowName.ToString());
		return false;
	}
}

void AIRPlayerController::UI_Close_Inventory()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->CloseInventoryWindow();
	}
}

void AIRPlayerController::UI_Close_Equipment()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->CloseEquipmentWindow();
	}
}

void AIRPlayerController::UI_Close_Container()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->CloseContainerWindow();
	}
}

void AIRPlayerController::UI_Set_IsMouseOverUI(bool bIsMouseOver)
{
	bIsMouseOverUI = bIsMouseOver;
}

FPlayerStats AIRPlayerController::UI_Get_PlayerStats()
{
	FPlayerStats LocalPlayerStats;
	if (InventoryManagerComponent)
	{
		LocalPlayerStats.Gold = InventoryManagerComponent->GetGold();
		LocalPlayerStats.Damage = InventoryManagerComponent->GetDamage();
		LocalPlayerStats.Armor = InventoryManagerComponent->GetArmor();
		LocalPlayerStats.Strength = InventoryManagerComponent->GetStrength();
		LocalPlayerStats.Dexterity = InventoryManagerComponent->GetDexterity();
		LocalPlayerStats.Intelligence = InventoryManagerComponent->GetIntelligence();
		LocalPlayerStats.InventorySize = InventoryManagerComponent->GetInventorySize();
		return LocalPlayerStats;
	}
	
	return LocalPlayerStats;
}

bool AIRPlayerController::UI_Get_IsShiftKeyDown()
{
	return bIsShiftKeyDown;
}

void AIRPlayerController::UI_Equip_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->EquipFromInventory(FromInvSlot, ToInvSlot);
	}
}

void AIRPlayerController::UI_UnEquip_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UnEquipFromInventory(FromInvSlot, ToInvSlot);
	}
}

void AIRPlayerController::UI_Split_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot, int32 Amount)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->SplitInventoryItem(FromInvSlot, ToInvSlot, Amount);
	}
}

void AIRPlayerController::UI_Move_Inventory_Item(int32 FromInvSlot, int32 ToInvSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->MoveInventoryItem(FromInvSlot, ToInvSlot);
	}

}

void AIRPlayerController::UI_Split_Item_From_Inventory(int32 FromInventorySlot, int32 ToSlot, int32 Amount)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->SplitInventoryItem(FromInventorySlot, ToSlot, Amount);
	}
}

void AIRPlayerController::UI_Split_Container_Item(int32 FromContainerSlot, int32 ToContainerSlot, int32 Amount)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->SplitContainerItem(FromContainerSlot, ToContainerSlot, Amount);
	}
}

void AIRPlayerController::UI_Move_Container_Item(int32 FromContainerSlot, int32 ToContainerSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->MoveContainerItem(FromContainerSlot, ToContainerSlot);
	}
}

void AIRPlayerController::UI_Take_Container_Item(int32 FromContainerSlot, int32 ToContainerSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->TakeContainerItem(FromContainerSlot, ToContainerSlot);
	}
}

void AIRPlayerController::UI_Deposit_Container_Item(int32 FromInvSlot, int32 ToContainerSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->DepositContainerItem(FromInvSlot, ToContainerSlot);
	}
}

void AIRPlayerController::UI_Split_Item_From_Container(int32 FromContainerSlot, int32 ToSlot, int32 Amount)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->SplitContainerItem(FromContainerSlot, ToSlot, Amount);
	}
}

void AIRPlayerController::UI_Use_Inventory_Item(int32 InvSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UseInventoryItem(InvSlot);
	}
}

void AIRPlayerController::UI_Use_Container_Item(int32 ContainerSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UseContainerItem(ContainerSlot);
	}

}

void AIRPlayerController::UI_Move_Hotbar_Item(int32 FromSlot, int32 ToSlot, bool bIsDraggedFromInventory, bool bIsDraggedFromHotbar)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->MoveHotbarSlotItem(FromSlot, ToSlot, bIsDraggedFromInventory, bIsDraggedFromHotbar);
	}
}

void AIRPlayerController::UI_Drop_Inventory_Item(int32 FromInvSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->DropItemFromInventory(FromInvSlot);
	}
}

void AIRPlayerController::UI_Clear_Hotbar_Item(int32 FromHotbarSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->ClearHotbarSlotItem(FromHotbarSlot);
	}
}

void AIRPlayerController::UI_Equip_From_Container(int32 FromContainerSlot, int32 ToInvSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->EquipFromContainer(FromContainerSlot, ToInvSlot);
	}
}

void AIRPlayerController::UI_UnEquip_To_Container(int32 FromInvSlot, int32 ToContainerSlot)
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UnEquipToContainer(FromInvSlot, ToContainerSlot);
	}
}

FInventoryItem AIRPlayerController::UI_Get_ToolTip_Info(FName ItemID)
{
	FInventoryItem LocToolTipItem;
	GetDataTableRowByName(ItemListDataTable, ItemID, LocToolTipItem);
	return LocToolTipItem;
}