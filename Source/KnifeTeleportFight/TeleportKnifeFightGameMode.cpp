// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportKnifeFightGameMode.h"

#include "EnemyCharacter.h"

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
	
	if (AEnemyCharacter* DeadCharacter = Cast<AEnemyCharacter>(DeadActor)) {
		DeadCharacter->HandleDeath();
	}
}


void ATeleportKnifeFightGameMode::BeginPlay() {
	Super::BeginPlay();

	HandleGameStart();
}

void ATeleportKnifeFightGameMode::HandleGameStart() {
	StartGame();

}