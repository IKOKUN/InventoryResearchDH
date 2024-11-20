// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IRCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

AIRCharacter::AIRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Chest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chest"));
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Shoulder = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoulder"));
	Waist = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Waist"));
	Back = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Back"));
	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Feet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	Accessory = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Accessory"));
	RightRing = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightRing"));
	LeftRing = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftRing"));
	MainHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MainHand"));
	OffHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OffHand"));
}

void AIRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AIRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

