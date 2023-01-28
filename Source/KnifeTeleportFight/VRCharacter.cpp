// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/PostProcessComponent.h"
#include "InputMappingContext.h"
#include "HeadMountedDisplayFunctionLibrary.h"


// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

	if (RightHandControllerClass)
	{
		RightHandController = GetWorld()->SpawnActor<AHandControllerBase>(RightHandControllerClass);
		RightHandController->AttachToComponent(VRRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
		RightHandController->SetHand(EControllerHand::Right);
		RightHandController->SetOwner(this);
	}

	if (LeftHandControllerClass)
	{
		LeftHandController = GetWorld()->SpawnActor<AHandControllerBase>(LeftHandControllerClass);
		LeftHandController->AttachToComponent(VRRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
		LeftHandController->SetHand(EControllerHand::Left);
		LeftHandController->SetOwner(this);
	}
	
	if (RightHandController && LeftHandController)
	{
		RightHandController->PairController(LeftHandController);
		LeftHandController->PairController(RightHandController);
	}

	// Spawn the knife in front of you
	if (TeleportKnifeClass)
	{
		TeleportKnife = GetWorld()->SpawnActor<ATeleportKnife>(TeleportKnifeClass);
		TeleportKnife->SetActorLocation(GetKnifeSpawnLocation());
		TeleportKnife->SetOwner(this);
	}

	// Input
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController != nullptr) {
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (!InputMapping.IsNull())
				{
					InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), /* Priority */ 0);
				}
			}
		}
	}
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(GrabLeftAction, ETriggerEvent::Started, this, &AVRCharacter::GrabLeft);
	Input->BindAction(GrabLeftAction, ETriggerEvent::Completed, this, &AVRCharacter::ReleaseLeft);
	Input->BindAction(GrabRightAction, ETriggerEvent::Started, this, &AVRCharacter::GrabRight);
	Input->BindAction(GrabRightAction, ETriggerEvent::Completed, this, &AVRCharacter::ReleaseRight);
	Input->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AVRCharacter::MoveForward);
	Input->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AVRCharacter::MoveRight);
	Input->BindAction(RecallWeaponAction, ETriggerEvent::Started, this, &AVRCharacter::RecallWeapon);
	Input->BindAction(TeleportAction, ETriggerEvent::Started, this, &AVRCharacter::Teleport);
}

void AVRCharacter::GrabLeft()
{
	LeftHandController->Grab();
}

void AVRCharacter::ReleaseLeft()
{
	LeftHandController->Release();
}

void AVRCharacter::GrabRight()
{
	RightHandController->Grab();
}

void AVRCharacter::ReleaseRight()
{
	RightHandController->Release();
}
 
void AVRCharacter::MoveForward(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Axis1D) return;
	
	AddMovementInput(Value.Get<float>() * Camera->GetForwardVector());
}

void AVRCharacter::MoveRight(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Axis1D) return;

	AddMovementInput(Value.Get<float>() * Camera->GetRightVector());
}

void AVRCharacter::RecallWeapon()
{
	TeleportKnife->Recall(GetKnifeSpawnLocation());
}

void AVRCharacter::Teleport()
{
}

FVector AVRCharacter::GetKnifeSpawnLocation()
{
	return GetActorLocation() + GetActorForwardVector() * KnifeSpawnDistance;
}

