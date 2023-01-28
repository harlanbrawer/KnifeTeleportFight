// Fill out your copyright notice in the Description page of Project Settings.


#include "HandControllerBase.h"

#include "MotionControllerComponent.h"

// Sets default values
AHandControllerBase::AHandControllerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
	MotionController->SetShowDeviceModel(true);
}

void AHandControllerBase::SetHand(EControllerHand Hand)
{
	MotionController->SetTrackingSource(Hand);
}

void AHandControllerBase::PairController(AHandControllerBase* Controller)
{
	OtherController = Controller;
}

void AHandControllerBase::Grab()
{
}

void AHandControllerBase::Release()
{
}

// Called when the game starts or when spawned
void AHandControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHandControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

