// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportKnifeFightGameMode.h"

#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "VRCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATeleportKnifeFightGameMode::ActorDied(AActor* DeadActor) {
	//if (DeadActor == Tank) {
	//	Tank->HandleDestruction();
	//	if (ToonTanksPlayerController) {
	//		ToonTanksPlayerController->SetPlayerEnabledState(false);
	//	}
	//	GameOver(false);
	//}
	//else if (ATower* DestroyedTower = Cast<ATower>(DeadActor)) {
	//	DestroyedTower->HandleDestruction();
	//	TargetTowers--;
	//	if (TargetTowers == 0) {
	//		GameOver(true);
	//	}
	//}
	//FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &AToonTanksGameMode::BeginPlay);\
	
	if (AEnemyCharacter* DeadEnemyCharacter = Cast<AEnemyCharacter>(DeadActor)) {
		DeadEnemyCharacter->HandleDeath();
		EndGame(true);
	}
	else if (AVRCharacter* DeadMainCharacter = Cast<AVRCharacter>(DeadActor)) {
		DeadMainCharacter->HandleDeath();
		EndGame(false);
	}
}


void ATeleportKnifeFightGameMode::BeginPlay() {
	Super::BeginPlay();

	Player1 = Cast<AVRCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Player2 = Cast<AEnemyCharacter>(UGameplayStatics::GetPlayerPawn(this, 1));

	HandleGameStart();
}

void ATeleportKnifeFightGameMode::HandleGameStart() {


	StartGame();
}

void ATeleportKnifeFightGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
