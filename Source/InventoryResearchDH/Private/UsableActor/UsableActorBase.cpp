#include "UsableActor/UsableActorBase.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
}

bool AUsableActorBase::OnActorUsed(APlayerController* PlayerController)
{
	bWasUsed = bWasUsed ? false : true;
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
