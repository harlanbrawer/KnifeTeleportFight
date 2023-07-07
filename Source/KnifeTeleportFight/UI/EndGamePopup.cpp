// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGamePopup.h"

// Sets default values
AEndGamePopup::AEndGamePopup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	VictoryPopup = CreateDefaultSubobject<UWidgetComponent>(TEXT("VictoryPopup"));
	VictoryPopup->SetupAttachment(RootComponent);

	DefeatPopup = CreateDefaultSubobject<UWidgetComponent>(TEXT("DefeatPopup"));
	DefeatPopup->SetupAttachment(RootComponent);
}

void AEndGamePopup::ShowVictoryMessage()
{
	VictoryPopup->SetHiddenInGame(false);
	DefeatPopup->SetHiddenInGame(true);
}

void AEndGamePopup::ShowDefeatMessage()
{
	VictoryPopup->SetHiddenInGame(true);
	DefeatPopup->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void AEndGamePopup::BeginPlay()
{
	Super::BeginPlay();

	DefeatPopup->SetHiddenInGame(true);
	VictoryPopup->SetHiddenInGame(true);
}

// Called every frame
void AEndGamePopup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

