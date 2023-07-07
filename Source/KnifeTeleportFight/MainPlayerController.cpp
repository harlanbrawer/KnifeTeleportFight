// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	//HUD = CreateWidget(this, HUDClass);
	//if (HUD != nullptr)
	//{
	//	HUD->AddToViewport();
	//}
}

void AMainPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	//HUD->RemoveFromViewport();

	EndGamePopup = GetWorld()->SpawnActor<AEndGamePopup>(EndGamePopupClass);

	if (EndGamePopup) {
		EndGamePopup->AttachToActor(GetPawn(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		EndGamePopup->SetOwner(this);
	}

	if (bIsWinner)
	{
		UE_LOG(LogTemp, Warning, TEXT("spawnning winner message"));
		EndGamePopup->ShowVictoryMessage();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("spawning defeat message"));
		EndGamePopup->ShowDefeatMessage();
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
