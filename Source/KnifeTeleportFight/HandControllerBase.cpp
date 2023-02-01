// Fill out your copyright notice in the Description page of Project Settings.


#include "HandControllerBase.h"

#include "MotionControllerComponent.h"
#include "GrabComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
	UGrabComponent* ComponentToGrab = GetGrabComponentNearMotionController();
	if (!ComponentToGrab) return;
	UE_LOG(LogTemp, Warning, TEXT("Got component"));

	bool bGrabSuccessful = ComponentToGrab->TryGrab(MotionController);
	if (!bGrabSuccessful) return;

	HeldComponent = ComponentToGrab;

	// If component is held by other hand, clear reference to it
	if (OtherController->HeldComponent == HeldComponent)
	{
		OtherController->HeldComponent = nullptr;
	}
}

void AHandControllerBase::Release()
{
	if (!HeldComponent) return;

	bool bReleaseSuccessful = HeldComponent->TryRelease(MotionController);
	if (!bReleaseSuccessful) return;

	HeldComponent = nullptr;

}

UGrabComponent* AHandControllerBase::GetGrabComponentNearMotionController()
{
	FVector HandLocation = GetActorLocation();
	
	TArray<FHitResult> OutHits;
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), HandLocation, HandLocation, GrabRadius, GrabbableObjectTypes, false, {}, EDrawDebugTrace::Persistent, OutHits, true);
	if (!bHit) return nullptr;
	if (!OutHits.Num()) return nullptr; // Not sure if this is necessary since bHit should be false I think

	UGrabComponent* ClosestGrabComponentInRange = nullptr;
	float DistanceToClosestGrabbableComponentInRange = GrabRadius + 10000; // Arbitary bigger than GrabRadius
	for (int i = 0; i < OutHits.Num(); i++)
	{
		AActor* HitActor = OutHits[i].GetActor();
		TArray<UActorComponent*> GrabComponentsOnActor = HitActor->GetComponentsByClass(UGrabComponent::StaticClass());
		for (int j = 0; j < GrabComponentsOnActor.Num(); j++)
		{
			UGrabComponent* HitComponent = Cast<UGrabComponent>(GrabComponentsOnActor[j]);
			float DistanceToComponent = (HitComponent->GetComponentLocation() - HandLocation).Length();
			if (DistanceToComponent < DistanceToClosestGrabbableComponentInRange) {
				ClosestGrabComponentInRange = HitComponent;
				DistanceToClosestGrabbableComponentInRange = DistanceToComponent;
			}
		}
	}

	return ClosestGrabComponentInRange;
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

