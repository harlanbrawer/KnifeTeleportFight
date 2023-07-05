// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "TeleportKnife.h"
#include "GrabComponent.h"
#include "Math/Vector.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

void AEnemyCharacter::HandleDeath()
{
	Destroy();
}

void AEnemyCharacter::ThrowRandomlyNearPlayer()
{
	if (!TeleportKnife->GetGrabComponent()->IsHeld()) return;

	// Randomly pick direction to throw in
	int32 RandomThrowIndex = FMath::RandRange(0, 3);
	Throw(static_cast<EThrowDirections>(RandomThrowIndex));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (TeleportKnifeClass)
	{
		TeleportKnife = GetWorld()->SpawnActor<ATeleportKnife>(TeleportKnifeClass);
		TeleportKnife->SetOwner(this);
		RecallWeapon();
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::RecallWeapon()
{
	if (TeleportKnife->GetGrabComponent()->IsHeld()) return;

	TeleportKnife->GetGrabComponent()->SocketGrab(GetMesh(), TEXT("middle_01_rSocket"));
}

void AEnemyCharacter::Throw(EThrowDirections Direction)
{
	if (!TeleportKnife->GetGrabComponent()->IsHeld()) return;
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	// Determine direction to throw the dagger in
	//FVector VectorToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	FVector VectorToPlayer = PlayerPawn->GetActorLocation() - TeleportKnife->GetActorLocation();
	FVector VectorToRightOfPlayer = FVector::CrossProduct(VectorToPlayer, FVector::UpVector);
	VectorToRightOfPlayer.Normalize();

	FVector ThrowVector;
	UE_LOG(LogTemp, Warning, TEXT("direction %d"), Direction);
	switch (Direction) {
		case EThrowDirections::Straight:
			ThrowVector = VectorToPlayer;
			break;
		case EThrowDirections::Up:
			ThrowVector = VectorToPlayer + FVector(0, 0, TeleportOffsetVertical);
			break;
		case EThrowDirections::Right:
			ThrowVector = VectorToPlayer + VectorToRightOfPlayer * TeleportOffsetHorizontal;
			break;
		case EThrowDirections::Left:
			ThrowVector = VectorToPlayer - VectorToRightOfPlayer * TeleportOffsetHorizontal;
			break;
	}

	DrawDebugLine(GetWorld(), TeleportKnife->GetActorLocation(), TeleportKnife->GetActorLocation() + ThrowVector, FColor::Red, true);

	// Launch the knife in the intended direction
	TeleportKnife->Launch(ThrowVector.GetSafeNormal() * ThrowSpeed);
} 

