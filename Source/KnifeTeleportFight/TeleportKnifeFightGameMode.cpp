// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportKnifeFightGameMode.h"

void ATeleportKnifeFightGameMode::ActorDied(AActor* DeadActor) {

}


void ATeleportKnifeFightGameMode::BeginPlay() {
	Super::BeginPlay();

	HandleGameStart();
}

void ATeleportKnifeFightGameMode::HandleGameStart() {
	StartGame();

}