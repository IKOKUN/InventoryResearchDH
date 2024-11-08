// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/IRPlayerController.h"

#include "HUD/IRHUD.h"
#include "InventoryComponent/EquipmentInventoryComponent.h"
#include "InventoryComponent/InventoryManagerComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AIRPlayerController::AIRPlayerController()
{
	bReplicates = true;

	PlayerInventoryComponent = CreateDefaultSubobject<UEquipmentInventoryComponent>(TEXT("PlayerInventoryComponent"));
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryManagerComponent"));
}

void AIRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController() == false) return;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	AIRHUD* IRHUD = Cast<AIRHUD>(GetHUD());
	check(IRHUD);

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	// InputModeData.SetWidgetToFocus(IRHUD->HUDReference);
	SetInputMode(InputModeData);


	APawn* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn)
	{
		UE_LOG(LogTemp, Log, TEXT("Pawn controlled by this PlayerController: %s"), *ControlledPawn->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Pawn is being controlled by this PlayerController"));
	}
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
		UE_LOG(LogTemp, Log, TEXT("Move function called"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No controlled pawn found."));
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
