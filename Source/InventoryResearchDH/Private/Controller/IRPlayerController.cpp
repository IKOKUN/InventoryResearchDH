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

	if (!bIsMovementLocked)
	{
		GetUsableActorFocus();
	}
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

		// Interacting
		EnhancedInputComponent->BindAction(UseActorAction, ETriggerEvent::Started, this, &AIRPlayerController::OnActorUsed);

		// Equipment and Inventory
		EnhancedInputComponent->BindAction(OpenEquipmentAndInventoryAction, ETriggerEvent::Started, this, &AIRPlayerController::OpenEquipmentAndInventory);
		UE_LOG(LogTemp, Log, TEXT("OpenEquipmentAndInventory action bound successfully."));

		// Equipment
		EnhancedInputComponent->BindAction(OpenEquipmentAction, ETriggerEvent::Started, this, &AIRPlayerController::OpenEquipment);

		// Inventory
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &AIRPlayerController::OpenInventory);

		// Hotbar
		EnhancedInputComponent->BindAction(Hotbar1Action, ETriggerEvent::Started, this, &AIRPlayerController::Hotbar1);
		EnhancedInputComponent->BindAction(Hotbar2Action, ETriggerEvent::Started, this, &AIRPlayerController::Hotbar2);
		EnhancedInputComponent->BindAction(Hotbar3Action, ETriggerEvent::Started, this, &AIRPlayerController::Hotbar3);
		EnhancedInputComponent->BindAction(Hotbar4Action, ETriggerEvent::Started, this, &AIRPlayerController::Hotbar4);
		EnhancedInputComponent->BindAction(Hotbar5Action, ETriggerEvent::Started, this, &AIRPlayerController::Hotbar5);
		EnhancedInputComponent->BindAction(Hotbar6Action, ETriggerEvent::Started, this, &AIRPlayerController::Hotbar6);
		EnhancedInputComponent->BindAction(Hotbar7Action, ETriggerEvent::Started, this, &AIRPlayerController::Hotbar7);
		EnhancedInputComponent->BindAction(Hotbar8Action, ETriggerEvent::Started, this, &AIRPlayerController::Hotbar8);
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

void AIRPlayerController::SteerCharacter(const FInputActionValue& Value)
{

}

void AIRPlayerController::OpenEquipmentAndInventory()
{
	if (InventoryManagerComponent)
	{
		if (InventoryManagerComponent->GetIsInventoryOpen())
		{
			InventoryManagerComponent->CloseInventoryWindow();
			InventoryManagerComponent->CloseEquipmentWindow();

			// UE_LOG(LogTemp, Error, TEXT("Close And Inventory"));
		}
		else
		{
			InventoryManagerComponent->OpenInventoryWindow();
			InventoryManagerComponent->OpenEquipmentWindow();

			// UE_LOG(LogTemp, Error, TEXT("Open Equipment And Inventory"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
	}
}

void AIRPlayerController::OpenEquipment()
{
	if (InventoryManagerComponent)
	{
		if (InventoryManagerComponent->GetIsEquipmentOpen())
		{
			InventoryManagerComponent->CloseEquipmentWindow();
		}
		else
		{
			InventoryManagerComponent->OpenEquipmentWindow();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
	}
}

void AIRPlayerController::OpenInventory()
{
	if (InventoryManagerComponent)
	{
		if (InventoryManagerComponent->GetIsInventoryOpen())
		{
			InventoryManagerComponent->CloseInventoryWindow();
		}
		else
		{
			InventoryManagerComponent->OpenInventoryWindow();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
	}
}

void AIRPlayerController::UseActor()
{
	OnActorUsed();
}

void AIRPlayerController::InspectActor()
{
}

void AIRPlayerController::Hotbar1()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UseHotbarSlot(0);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
	}
}

void AIRPlayerController::Hotbar2()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UseHotbarSlot(1);
		UE_LOG(LogTemp, Error, TEXT("Open Hotbar Slot"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
	}
}

void AIRPlayerController::Hotbar3()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UseHotbarSlot(2);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
	}
}

void AIRPlayerController::Hotbar4()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UseHotbarSlot(3);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
	}
}

void AIRPlayerController::Hotbar5()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UseHotbarSlot(4);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
	}
}

void AIRPlayerController::Hotbar6()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UseHotbarSlot(5);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
	}
}

void AIRPlayerController::Hotbar7()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UseHotbarSlot(6);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
	}
}

void AIRPlayerController::Hotbar8()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->UseHotbarSlot(7);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryManagerComponent is not valid"));
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
			//UE_LOG(LogTemp, Log, TEXT("InteractText is now visible with message: %s"), *HUDReference->InteractText->MessageText.ToString());
		}
		else
		{
			HUDReference->InteractText->SetVisibility(ESlateVisibility::Hidden);
			//UE_LOG(LogTemp, Log, TEXT("InteractText is empty, setting visibility to Hidden"));
		}

		// Log untuk memeriksa visibilitas widget
		ESlateVisibility CurrentVisibility = HUDReference->InteractText->GetVisibility();
		//UE_LOG(LogTemp, Log, TEXT("Current visibility of InteractText: %d"), (int32)CurrentVisibility);
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
	// DrawDebugLine(PlayerCharacter->GetWorld(), Start, End, FColor::Green, false, 2.0f);

	// 6. Jika Line Trace mengenai sesuatu
	if (bHit && HitResult.GetActor())
	{
		AUsableActorBase* UsableActor = Cast<AUsableActorBase>(HitResult.GetActor());
		if (UsableActor)
		{
			// UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
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
	AUsableActorBase* CurrentUsableActor = GetUsableActor();

	// Jika ada aktor yang dapat digunakan
	if (CurrentUsableActor)
	{
		// Jika aktor baru ditemukan dan berbeda dari LastUsableActor
		if (CurrentUsableActor != LastUsableActor)
		{
			bIsNewFocus = true;

			// Akhiri fokus pada aktor sebelumnya
			if (LastUsableActor)
			{
				LastUsableActor->EndOutlineFocus();
			}
		}

		// Set LastUsableActor ke aktor yang baru ditemukan
		LastUsableActor = CurrentUsableActor;

		// Jika ini adalah fokus baru
		if (bIsNewFocus)
		{
			LastUsableActor->BeginOutlineFocus();
			bIsNewFocus = false;

			// Atur teks interaksi
			FText ActionText = GetActorActionText(LastUsableActor);
			SetInteractText(ActionText);
			ShowInteractText();
		}
	}
	else
	{
		// Jika tidak ada aktor yang dapat digunakan, sembunyikan teks interaksi
		if (LastUsableActor)
		{
			LastUsableActor->EndOutlineFocus();
			LastUsableActor = nullptr; // Reset LastUsableActor
		}
		SetInteractText(FText());
		HideInteractText();
	}

	// Cek apakah LastUsableActor masih valid dan dapat digunakan
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
		// return;
	}

	// Initialize HUDReference
	if (MainHUDWidgetClass)
	{
		HUDReference = CreateWidget<UHUDLayoutWidget>(GetWorld(), MainHUDWidgetClass);
		if (HUDReference)
		{
			// Menambahkan widget ke viewport
			HUDReference->AddToViewport();
		}
	}

	if (!HUDReference)
	{
		UE_LOG(LogTemp, Error, TEXT("HUDReference is not valid"));
		return;
	}

	// Check if this is a local player controller
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Configure input mode and mouse cursor
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::GrabHand;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);

	if (HUDReference->IsValidLowLevel())
	{
		TSharedPtr<SWidget> HUDRefTakeWidget = HUDReference->TakeWidget();
		InputModeData.SetWidgetToFocus(HUDRefTakeWidget);
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
	// Menggunakan StaticEnum untuk mendapatkan pointer ke enum
	UEnum* EnumPtr = StaticEnum<EClassesHero>();
	if (!EnumPtr) return FName("Invalid");

	// Mengonversi enum ke FName berdasarkan nilai
	return EnumPtr->GetNameByValue((int64)EnumValue);
}

void AIRPlayerController::LoadPlayerItems()
{
	TArray<FName> LocalItemIds;
	TArray<FInventoryItem> LocalInventoryItems;
	FName HeroCategory = FName("Warrior");
	EClassesHero LocalHeroClass = EClassesHero::Warrior;
	FNPCItems LocalNPCItems;

	int32 RandomInteger = FMath::RandRange(0, 3 - 1);
	LocalHeroClass = static_cast<EClassesHero>(RandomInteger);

	FName RowName = EnumToFNameByValue(LocalHeroClass);
	//UE_LOG(LogTemp, Warning, TEXT("Trying to find row: %s"), *RowName.ToString());

	if (GetDataTableRowByName(ClassStartingEquipment, HeroCategory, LocalNPCItems))
	{
		// Tambahkan item ke LocalItemIds
		if (LocalNPCItems.Head != NAME_None) LocalItemIds.Add(LocalNPCItems.Head);
		if (LocalNPCItems.Chest != NAME_None) LocalItemIds.Add(LocalNPCItems.Chest);
		if (LocalNPCItems.Legs != NAME_None) LocalItemIds.Add(LocalNPCItems.Legs);
		if (LocalNPCItems.Feet != NAME_None) LocalItemIds.Add(LocalNPCItems.Feet);
		if (LocalNPCItems.Accessory != NAME_None) LocalItemIds.Add(LocalNPCItems.Accessory);
		if (LocalNPCItems.Back != NAME_None) LocalItemIds.Add(LocalNPCItems.Back);
		if (LocalNPCItems.RightRing != NAME_None) LocalItemIds.Add(LocalNPCItems.RightRing);
		if (LocalNPCItems.LeftRing != NAME_None) LocalItemIds.Add(LocalNPCItems.LeftRing);
		if (LocalNPCItems.Waist != NAME_None) LocalItemIds.Add(LocalNPCItems.Waist);
		if (LocalNPCItems.Trinket != NAME_None) LocalItemIds.Add(LocalNPCItems.Trinket);
		if (LocalNPCItems.MainHand != NAME_None) LocalItemIds.Add(LocalNPCItems.MainHand);
		if (LocalNPCItems.OffHand != NAME_None) LocalItemIds.Add(LocalNPCItems.OffHand);
		if (LocalNPCItems.Shoulders != NAME_None) LocalItemIds.Add(LocalNPCItems.Shoulders);
		if (LocalNPCItems.Hands != NAME_None) LocalItemIds.Add(LocalNPCItems.Hands);

		// Loop Pertama
		FInventoryItem LocalInventoryItem1;
		int32 LocalIndex = 0;
		for (FName itmid : LocalItemIds)
		{
			if (itmid != NAME_None) // Pastikan itmid tidak None
			{
				// UE_LOG(LogTemp, Warning, TEXT("Trying to find item in loop 1: %s"), *itmid.ToString());
				if (GetDataTableRowByName(ItemListDataTable, itmid, LocalInventoryItem1))
				{
					if (SetInventoryArrayElement(LocalInventoryItems, LocalIndex, LocalInventoryItem1, true))
					{
						// UE_LOG(LogTemp, Warning, TEXT("Successfully added item: %s to LocalInventoryItems"), *LocalInventoryItem1.ID.ToString());
					}
					else
					{
						// UE_LOG(LogTemp, Warning, TEXT("Failed to add item: %s to LocalInventoryItems"), *LocalInventoryItem1.ID.ToString());
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Item not found in loop 1: %s"), *itmid.ToString());
					SetInventoryArrayElement(LocalInventoryItems, LocalIndex, FInventoryItem(), true);
				}
			}
			else
			{
				// UE_LOG(LogTemp, Warning, TEXT("Item ID is None in loop 1, skipping."));
			}
			LocalIndex++;
		}

		// Loop Kedua
		FInventoryItem LocalInventoryItem2;
		// Loop kedua
		for (FItem LocItem : LocalNPCItems.Inventory)
		{
			if (LocItem.ItemID != NAME_None) // Pastikan ItemID tidak None
			{
				// UE_LOG(LogTemp, Warning, TEXT("Trying to find inventory item ID in loop 2: %s"), *LocItem.ItemID.ToString());
				if (GetDataTableRowByName(ItemListDataTable, LocItem.ItemID, LocalInventoryItem2))
				{
					LocalInventoryItem2.Amount = LocItem.Amount;
					if (LocalInventoryItem2.ItemType == EItemType::Currency)
					{
						InventoryManagerComponent->TryToAddItemToInventory(PlayerInventoryComponent, LocalInventoryItem2);
						// Log untuk menunjukkan bahwa item currency berhasil ditambahkan
						// UE_LOG(LogTemp, Warning, TEXT("Added currency item: %s with amount in loop 2: %d"), *LocalInventoryItem2.ID.ToString(), LocalInventoryItem2.Amount);
					}
					else
					{
						LocalInventoryItems.Add(LocalInventoryItem2);
						// Log untuk menunjukkan bahwa item berhasil ditambahkan ke inventory
						// UE_LOG(LogTemp, Warning, TEXT("Successfully added item in loop 2: %s to LocalInventoryItems"), *LocalInventoryItem2.ID.ToString());
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Inventory item in loop 2 not found: %s"), *LocItem.ItemID.ToString());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Inventory item ID in loop 2 is None, skipping."));
			}
		}

		PlayerInventoryComponent->LoadInventoryItems(LocalInventoryItems.Num(), LocalInventoryItems);

		// Meng-equip item setelah memuat inventory
		/*for (int32 i = 0; i < LocalInventoryItems.Num(); i++)
		{
			FInventoryItem& Item = LocalInventoryItems[i];
			if (Item.ItemType == EItemType::Equipment)
			{
				int32 EquipmentSlotIndex = InventoryManagerComponent->GetEquipmentSlotByType(InventoryManagerComponent->ItemEquipSlot(Item));
				if (EquipmentSlotIndex != INDEX_NONE)
				{
					InventoryManagerComponent->EquipFromInventory(i, EquipmentSlotIndex);
				}
			}
		}*/
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Row not found: %s"), *HeroCategory.ToString());
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

	// UE_LOG(LogTemp, Warning, TEXT("UI Get Tool Tip Info Called"));
	return LocToolTipItem;
}