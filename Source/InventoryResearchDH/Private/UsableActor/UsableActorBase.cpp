#include "UsableActor/UsableActorBase.h"
#include "Controller/IRPlayerController.h"
#include "Character/IRCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "HUD/InspectWidget.h"
#include "GameFramework/PawnMovementComponent.h"

AUsableActorBase::AUsableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
}

bool AUsableActorBase::OnWasUsed()
{
	if (UsedSound)
	{
        //UGameplayStatics::PlaySoundAtLocation(this, UsedSound, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(this, UsedSound, GetActorLocation());
	}
	
	return true;
}

// Called when the game starts or when spawned
void AUsableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	// InteractorPlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	// InteractorPlayerCharacter = Cast<AIRCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

bool AUsableActorBase::OnActorUsed(APlayerController* PlayerController)
{
	SetOwner(PlayerController->GetPawn());
	InteractorPlayerController = Cast<AIRPlayerController>(PlayerController);
	InteractorPlayerCharacter = Cast<AIRCharacter>(InteractorPlayerController->GetPawn());

	bWasUsed = bWasUsed ? false : true;
	return true;
}

bool AUsableActorBase::OnActorInspect(APlayerController* PlayerController)
{
	SetOwner(PlayerController->GetPawn());
	InteractorPlayerController = Cast<AIRPlayerController>(PlayerController);
	InteractorPlayerCharacter = Cast<AIRCharacter>(InteractorPlayerController->GetPawn());

	if (!bGettingInspect)
	{
		bGettingInspect = true;
		InspectItem();
		UE_LOG(LogTemp, Warning, TEXT("Inspect Item!"));
	}
	else
	{
		bGettingInspect = false;
		EndInspection();
		UE_LOG(LogTemp, Warning, TEXT("End Inspect Item"));
	}
	return true;
}

bool AUsableActorBase::BeginOutlineFocus()
{
	StaticMeshComponent->SetRenderCustomDepth(true);
	return true;
}

bool AUsableActorBase::EndOutlineFocus()
{
	StaticMeshComponent->SetRenderCustomDepth(false);
	return true;
}

bool AUsableActorBase::GetIsActorUsable()
{
	return bIsUsable;
}

FText AUsableActorBase::GetUseActionText()
{
	FText ActionNameText = FText::Format(FText::FromString("{Action} {Name}"), ActionText, FText::FromName(Name));
	return ActionNameText;
}

void AUsableActorBase::InspectItem()
{
	// Mengatur timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitForCamera, this, &AUsableActorBase::WaitForCamera, 0.01f, true, 0.f);
	InteractorPlayerCharacter->GetMovementComponent()->StopMovementImmediately();
	//InteractorPlayerCharacter->DisableInput(InteractorPlayerController);
	// UE_LOG(LogTemp, Log, TEXT("Timer started with interval: %f seconds"), TimeInterval);
}

// ...

void AUsableActorBase::WaitForCamera()
{
	if (InteractorPlayerController)
	{
		// Mendapatkan Player Camera Manager
		IRCameraManager = InteractorPlayerController->PlayerCameraManager;
		if (IRCameraManager)
		{
			// Sekarang Anda dapat menggunakan CameraManager
			UE_LOG(LogTemp, Log, TEXT("Player Camera Manager found: %s"), *IRCameraManager->GetName());

			// Mendapatkan transform kamera
			FTransform CameraTransform = IRCameraManager->GetTransformComponent()->GetComponentTransform();
			if (UKismetMathLibrary::NearlyEqual_TransformTransform(CameraTransform, CameraTransformCache, 0.01f, 0.1f, 1.f))
			{
				// Kamera tidak bergerak
				// UE_LOG(LogTemp, Log, TEXT("Camera not moving"));
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle_WaitForCamera);
				StartInspection();
			}
			else
			{
				// Kamera bergerak
				UE_LOG(LogTemp, Log, TEXT("Camera moving"));
				CameraTransformCache = CameraTransform;
			}
		}
		else
		{
			IRCameraManager = InteractorPlayerController->PlayerCameraManager;
			WaitForCamera();
		}
	}
	
}

void AUsableActorBase::StartInspection()
{
	HoldFocusTime = -1.f;
	
	// Disable Collision dan Hide Character
	if (InteractorPlayerCharacter)
	{
		InteractorPlayerCharacter->SetActorHiddenInGame(true);
		InteractorPlayerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		InteractorPlayerCharacter->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		// StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);

		CameraFocus();

		// Save Origin Location and Rotation Depending On Simulation Physic Is Enabled
		ObjectOriginRotation = GetActorRotation();
		ObjectOriginLocation = GetActorLocation();

		// Center Object In Scene
		/*OriginRelativeLocation = StaticMeshComponent->GetRelativeLocation();
		StaticMeshComponent->SetRelativeLocation(FVector::ZeroVector);
		FVector LocalMin;
		FVector LocalMax;
		StaticMeshComponent->GetLocalBounds(LocalMin, LocalMax);
		StaticMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, LocalMax.Z / 2.f));*/

		if (InspectWidgetClass)
		{
			CurrentInspectWidget = CreateWidget<UInspectWidget>(InteractorPlayerController, InspectWidgetClass);
			if (CurrentInspectWidget)
			{
				CurrentInspectWidget->InvSlotItemInformation.ID = Name;
				CurrentInspectWidget->AddToViewport();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Inspect Widget not found!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Inspect Widget Class not found!"));
		}
		

		/*UE_LOG(LogTemp, Log, TEXT("Restoring Object to Location: %s, Rotation: %s"),
			*ObjectOriginLocation.ToString(), *ObjectOriginRotation.ToString());*/

		if (IRCameraManager)
		{
			FVector NewStartLocation = IRCameraManager->GetCameraLocation();
			FVector NewEndLocation = IRCameraManager->GetActorForwardVector() * InspectionDistance + NewStartLocation;

			SetActorLocationAndRotation(NewEndLocation, ObjectOriginRotation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Camera Manager not found!"));
		}
		InteractorPlayerCharacter->EnableInput(InteractorPlayerController);

	}
}

void AUsableActorBase::CameraFocus()
{
	
	if (InteractorPlayerCharacter && InteractorPlayerCharacter->GetMovementComponent() && InteractorPlayerController)
	{
		InteractorPlayerCharacter->GetMovementComponent()->StopMovementImmediately();
		// InteractorPlayerCharacter->DisableInput(InteractorPlayerController);

		ControlRotationOrigin = InteractorPlayerController->GetControlRotation();
		
		APlayerCameraManager* CameraManager = InteractorPlayerController->PlayerCameraManager;
		if (CameraManager)
		{
			TargetControlRotation = UKismetMathLibrary::FindLookAtRotation(CameraManager->GetCameraLocation(), GetActorLocation());
			InteractorPlayerController->SetControlRotation(UKismetMathLibrary::RLerp(ControlRotationOrigin, TargetControlRotation, 1.f, true));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Camera Manager not found!"));
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Pawn or Player Controller not found!"));
	}
}

void AUsableActorBase::EndInspection()
{
	check(InteractorPlayerCharacter && InteractorPlayerController && IRCameraManager);

	if (CurrentInspectWidget)
	{
		CurrentInspectWidget->RemoveFromParent();
		CurrentInspectWidget = nullptr;
	}
	

	// Set Origin Location (Relative) to Origin
	StaticMeshComponent->SetRelativeLocation(OriginRelativeLocation);

	// Disable Inspection Input and safe last location
	InspectionLocation = GetActorLocation();
	
	SetActorLocationAndRotation(ObjectOriginLocation, ObjectOriginRotation);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	InteractorPlayerCharacter->SetActorHiddenInGame(false);
	InteractorPlayerCharacter->EnableInput(InteractorPlayerController);
}

void AUsableActorBase::RotateObjectX(float AxisValue)
{
	FRotator ObjectRotation = FRotator(0.f, AxisValue * -10.f, 0.0f);
	AddActorWorldRotation(ObjectRotation);
}

void AUsableActorBase::RotateObjectY(float AxisValue)
{
	if (IRCameraManager)
	{
        FRotator CameraRotation = IRCameraManager->GetCameraRotation();
		bool bInRangeMinus90To0 = UKismetMathLibrary::InRange_FloatFloat(CameraRotation.Yaw, -90.f, 0.f);
		bool bInRangeMinus180ToMinus90 = UKismetMathLibrary::InRange_FloatFloat(CameraRotation.Yaw, -180.f, -90.f);
		bool bInRange90To180 = UKismetMathLibrary::InRange_FloatFloat(CameraRotation.Yaw, 90.f, 180.f);

		if (bInRangeMinus90To0) RotateYQuarter = 1;
		else if (bInRangeMinus180ToMinus90) RotateYQuarter = 2;
		else if (bInRangeMinus180ToMinus90) RotateYQuarter = 3;
		else RotateYQuarter = 0;

		switch (RotateYQuarter)
		{
		case 0:
			RotationFactorX = UKismetMathLibrary::MapRangeClamped(CameraRotation.Yaw, 0.f, 90.f, 0.f, -1.f);
		case 1:
			RotationFactorX = UKismetMathLibrary::MapRangeClamped(CameraRotation.Yaw, -90.f, 0.f, 1.f, 0.f);
		case 2:
			RotationFactorX = UKismetMathLibrary::MapRangeClamped(CameraRotation.Yaw, -90.f, -180.f, 1.f, 0.f);
		case 3:
			RotationFactorX = UKismetMathLibrary::MapRangeClamped(CameraRotation.Yaw, 90.f, 180.f, -1.f, 0.f);
		default:
			break;
		}

		switch (RotateYQuarter)
		{
		case 0:
			RotationFactorY = UKismetMathLibrary::MapRangeClamped(CameraRotation.Yaw, 0.f, 90.f, 1.f, 0.f);
		case 1:
			RotationFactorY = UKismetMathLibrary::MapRangeClamped(CameraRotation.Yaw, 0.f, -90.f, 1.f, 0.f);
		case 2:
			RotationFactorY = UKismetMathLibrary::MapRangeClamped(CameraRotation.Yaw, -90.f, -180.f, 0.f, -1.f);
		case 3:
			RotationFactorY = UKismetMathLibrary::MapRangeClamped(CameraRotation.Yaw, 90.f, 180.f, 0.f, 1.f);
		default:
			break;
		}

		NewObjectRotation = FRotator(AxisValue * RotationFactorY * 5.f, 0.f, AxisValue * RotationFactorX * 5.f);
		AddActorWorldRotation(NewObjectRotation);
	}
}

bool AUsableActorBase::GetDataTableRowByName(UDataTable* SrcDataTable, const FName RowName, FInventoryItem& OutInvItemRow)
{
	if (!SrcDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
		return false;
	}

	FInventoryItem* Row = SrcDataTable->FindRow<FInventoryItem>(RowName, TEXT(""));

	if (Row)
	{
		OutInvItemRow = *Row;
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Row not found: %s"), *RowName.ToString());
		return false;
	}
}

bool AUsableActorBase::GetDataTableRowByName(UDataTable* SrcDataTable, const FName RowName, FLootList& OutLootListRow)
{
	if (!SrcDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
		return false;
	}

	FLootList* Row = SrcDataTable->FindRow<FLootList>(RowName, TEXT(""));

	if (Row)
	{
		OutLootListRow = *Row;
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Row not found: %s"), *RowName.ToString());
		return false;
	}
}
