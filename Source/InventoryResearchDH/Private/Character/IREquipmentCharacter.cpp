#include "Character/IREquipmentCharacter.h"

#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h" // Include the header for USpringArmComponent
#include "Kismet/GameplayStatics.h"

AIREquipmentCharacter::AIREquipmentCharacter()
{
    // Buat komponen SceneCaptureComponent2D
    SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));

    // Attach ke root atau komponen lain (misalnya, mesh atau kamera)
    SceneCaptureComponent->SetupAttachment(GetCameraBoom(), USpringArmComponent::SocketName);

    // Atur Primitive Render Mode
    SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

    // Deactive Collison Test For SceneCaptureComponent
	// SceneCaptureComponent->bCaptureEveryFrame = true;              // Capture setiap frame

    // Set the Show Only Actors list
    SceneCaptureComponent->ShowOnlyActorComponents(this, true);
}

void AIREquipmentCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIREquipmentCharacter::UpdateEquipmentMesh(bool bIsEquip)
{
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<AIRCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (PlayerCharacter)
		{
			PlayerCharacter->RecordCharacter = this;
		}
	}
	else
	{
		PlayerCharacter->RecordCharacter = this;
	}

	UE_LOG(LogTemp, Warning, TEXT("UpdateEquipmentMesh In Record Character"));

	if (bIsEquip)
	{
		if (HeadMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("Head Mesh : %s"), *HeadMesh->GetName());
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

void AIREquipmentCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	/*PlayerCharacter = Cast<AIRCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (PlayerCharacter)
    {
        PlayerCharacter->RecordCharacter = this;
    }*/
}
