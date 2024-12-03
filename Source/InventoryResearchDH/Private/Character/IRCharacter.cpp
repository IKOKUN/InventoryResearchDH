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
	Head->SetupAttachment(GetMesh());
	Chest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chest"));
	Chest->SetupAttachment(GetMesh());
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Hands->SetupAttachment(GetMesh());
	Shoulder = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoulder"));
	Shoulder->SetupAttachment(GetMesh());
	Waist = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Waist"));
	Waist->SetupAttachment(GetMesh());
	Back = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Back"));
	Back->SetupAttachment(GetMesh());
	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(GetMesh());
	Feet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	Feet->SetupAttachment(GetMesh());
	Accessory = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Accessory"));
	Accessory->SetupAttachment(GetMesh());
	RightRing = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightRing"));
	RightRing->SetupAttachment(GetMesh());
	LeftRing = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftRing"));
	LeftRing->SetupAttachment(GetMesh());
	MainHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MainHand"));
	MainHand->SetupAttachment(GetMesh());
	OffHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OffHand"));
	OffHand->SetupAttachment(GetMesh());
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

void AIRCharacter::UpdateEquipmentMesh(bool bIsEquip)
{
	if (bIsEquip)
	{
		if (HeadMesh)
		{
			Head->SetSkeletalMesh(HeadMesh);
			Head->LeaderPoseComponent = GetMesh();
		}

		if (ChestMesh)
		{
			Chest->SetSkeletalMesh(ChestMesh);
			Chest->LeaderPoseComponent = GetMesh();
		}

		if (HandsMesh)
		{
			Hands->SetSkeletalMesh(HandsMesh);
			Hands->LeaderPoseComponent = GetMesh();
		}

		if (ShoulderMesh)
		{
			Shoulder->SetSkeletalMesh(ShoulderMesh);
			Shoulder->LeaderPoseComponent = GetMesh();
		}

		if (WaistMesh)
		{
			Waist->SetSkeletalMesh(WaistMesh);
			Waist->LeaderPoseComponent = GetMesh();
		}

		if (BackMesh)
		{
			Back->SetSkeletalMesh(BackMesh);
			Back->LeaderPoseComponent = GetMesh();
		}

		if (LegsMesh)
		{
			Legs->SetSkeletalMesh(LegsMesh);
			Legs->LeaderPoseComponent = GetMesh();
		}

		if (FeetMesh)
		{
			Feet->SetSkeletalMesh(FeetMesh);
			Feet->LeaderPoseComponent = GetMesh();
		}

		if (AccessoryMesh)
		{
			Accessory->SetSkeletalMesh(AccessoryMesh);
			Accessory->LeaderPoseComponent = GetMesh();
		}

		if (RightRingMesh)
		{
			RightRing->SetSkeletalMesh(RightRingMesh);
			RightRing->LeaderPoseComponent = GetMesh();
		}

		if (LeftRingMesh)
		{
			LeftRing->SetSkeletalMesh(LeftRingMesh);
			LeftRing->LeaderPoseComponent = GetMesh();
		}

		if (MainHandMesh)
		{
			MainHand->SetSkeletalMesh(MainHandMesh);
			MainHand->LeaderPoseComponent = GetMesh();
		}

		if (OffHandMesh)
		{
			OffHand->SetSkeletalMesh(OffHandMesh);
			OffHand->LeaderPoseComponent = GetMesh();
		}
	}
	else
	{
		if (Head)
		{
			Head->SetSkeletalMesh(nullptr);
		}
		if (Chest)
		{
			Chest->SetSkeletalMesh(nullptr);
		}
		if (Hands)
		{
			Hands->SetSkeletalMesh(nullptr);
		}
		if (Shoulder)
		{
			Shoulder->SetSkeletalMesh(nullptr);
		}
		if (Waist)
		{
			Waist->SetSkeletalMesh(nullptr);
		}
		if (Back)
		{
			Back->SetSkeletalMesh(nullptr);
		}
		if (Legs)
		{
			Legs->SetSkeletalMesh(nullptr);
		}
		if (Feet)
		{
			Feet->SetSkeletalMesh(nullptr);
		}
		if (Accessory)
		{
			Accessory->SetSkeletalMesh(nullptr);
		}
		if (RightRing)
		{
			RightRing->SetSkeletalMesh(nullptr);
		}
		if (LeftRing)
		{
			LeftRing->SetSkeletalMesh(nullptr);
		}
		if (MainHand)
		{
			MainHand->SetSkeletalMesh(nullptr);
		}
		if (OffHand)
		{
			OffHand->SetSkeletalMesh(nullptr);
		}

		HeadMesh = nullptr;
		ChestMesh = nullptr;
		HandsMesh = nullptr;
		ShoulderMesh = nullptr;
		WaistMesh = nullptr;
		BackMesh = nullptr;
		LegsMesh = nullptr;
		FeetMesh = nullptr;
		AccessoryMesh = nullptr;
		RightRingMesh = nullptr;
		LeftRingMesh = nullptr;
		MainHandMesh = nullptr;
		OffHandMesh = nullptr;
	}
}

