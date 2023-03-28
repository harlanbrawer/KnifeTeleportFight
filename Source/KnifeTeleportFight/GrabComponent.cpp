// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"

#include "Components/PrimitiveComponent.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UGrabComponent::TryGrab(UMotionControllerComponent* GrabbingMotionControllerComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("TRY grab"));
	// Get parent primitive component
	USceneComponent* ParentComponent = GetAttachParent();
	if (!ParentComponent) return false;
	UPrimitiveComponent* PrimitiveParent = Cast<UPrimitiveComponent>(ParentComponent);
	if (!PrimitiveParent) return false;

	PrimitiveParent->SetSimulatePhysics(false);
	
	// Attach parent component to the motion controller component
	USceneComponent* UMotionControllerSceneComponent = Cast<USceneComponent>(GrabbingMotionControllerComponent);
	GetOwner()->AttachToActor(UMotionControllerSceneComponent->GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	bIsHeld = true; 

	// Set rotation to be up in the hand
	UArrowComponent* UpDirectionComponent = Cast<UArrowComponent>(GrabbingMotionControllerComponent->GetOwner()->GetComponentByClass(UArrowComponent::StaticClass()));
	if (UpDirectionComponent)
	{
		GetOwner()->SetActorRotation(UpDirectionComponent->GetComponentRotation());
		//GetOwner()->AddActorLocalRotation(FRotator(-90, 0, 0));
	}

	// Set new location
	FVector GrabComponentInverseOffset = GetOwner()->GetActorLocation() - GetComponentLocation();
	FVector NewParentComponentLocation = GrabbingMotionControllerComponent->GetOwner()->GetActorLocation() + GrabComponentInverseOffset;
	GetOwner()->SetActorLocation(NewParentComponentLocation, false, nullptr, ETeleportType::ResetPhysics);

	// Store ref to controller holding this object
	HoldingControllerComponentRef = GrabbingMotionControllerComponent;

	// Call virtual OnGrabbed method for inherited objects or Dispatch OnGrabbed event for blueprint extensions
	OnGrabbed();

	// Play haptic effect
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->PlayHapticEffect(HapticEffect, GrabbingMotionControllerComponent->GetTrackingSource());

	return true;
}

bool UGrabComponent::TryRelease(UMotionControllerComponent* GrabbingMotionControllerComponent)
{
	// Get parent primitive component
	USceneComponent* ParentComponent = GetAttachParent();
	if (!ParentComponent) return false;
	UPrimitiveComponent* PrimitiveParent = Cast<UPrimitiveComponent>(ParentComponent);
	if (!PrimitiveParent) return false;

	// Start simulating physics to detatch from hand
	PrimitiveParent->SetSimulatePhysics(true);

	// No longer held
	bIsHeld = false;

	// Call virtual OnDropped
	OnDropped();

	return true;
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

