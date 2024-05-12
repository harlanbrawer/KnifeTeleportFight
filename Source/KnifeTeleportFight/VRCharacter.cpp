// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/PostProcessComponent.h"
#include "InputMappingContext.h"
#include "Kismet/KismetMathLibrary.h"
#include "CustomUtils.h"
#include "Components/InputComponent.h"
#include "TeleportKnife.h"

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

void AVRCharacter::HandleDeath()
{
	Super::HandleDeath();
	//SetActorHiddenInGame(true);
	//SetActorTickEnabled(false);
	//bAlive = false;
}

FTransform AVRCharacter::GetCameraTransform()
{
	if (!Camera) return FTransform::Identity;
	return Camera->GetComponentToWorld();
}

FTransform AVRCharacter::GetLeftControllerTransform()
{
	if (!LeftHandController) return FTransform::Identity;
	return LeftHandController->GetActorTransform();
}

FTransform AVRCharacter::GetRightControllerTransform()
{
	if (!RightHandController) return FTransform::Identity;
	return RightHandController->GetActorTransform();
}

UCameraComponent* AVRCharacter::GetCamera()
{
	return Camera;
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

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
	if (TeleportKnife)
	{
		//TeleportKnife = GetWorld()->SpawnActor<ATeleportKnife>(TeleportKnifeClass);
		TeleportKnife->SetOwner(this);
		RecallWeapon();
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

	//FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	//NewCameraOffset.Z = 0;
	//AddActorWorldOffset(NewCameraOffset);
	//VRRoot->AddWorldOffset(-NewCameraOffset);
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
	
	// Keep track of when teleport is prepped
	Input->BindAction(PrepTeleGrabRightAction, ETriggerEvent::Started, this, &AVRCharacter::PrepTeleGrabRight);
	Input->BindAction(PrepTeleGrabLeftAction, ETriggerEvent::Started, this, &AVRCharacter::PrepTeleGrabLeft);
	Input->BindAction(PrepTeleGrabRightAction, ETriggerEvent::Completed, this, &AVRCharacter::UnprepTeleGrabRight);
	Input->BindAction(PrepTeleGrabLeftAction, ETriggerEvent::Completed, this, &AVRCharacter::UnprepTeleGrabLeft);
}

void AVRCharacter::GrabLeft()
{
	if (bInPrepTeleportLeft)
	{
		TeleGrabLeft();
	}
	else
	{
		LeftHandController->Grab();
	}
}

void AVRCharacter::ReleaseLeft()
{
	LeftHandController->Release();
}

void AVRCharacter::GrabRight()
{
	if (bInPrepTeleportRight)
	{
		TeleGrabRight();
	}
	else
	{
		RightHandController->Grab();
	}
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
	TeleportKnife->Recall(GetKnifeSpawnLocation(), GetKnifeSpawnRotation());
}

void AVRCharacter::Teleport()
{
}

void AVRCharacter::TeleGrabRight()
{
	// Get knife location
	FVector KnifeLocation = TeleportKnife->GetActorLocation();

	// Determine character teleport location
		// if knife too low then set knife pos up
	// Get current character height
	FVector CameraToHand = RightHandController->GetActorLocation() - Camera->GetComponentLocation();
	FVector CameraTeleportLocation = KnifeLocation - CameraToHand;
	
	FVector ActorToCamera = Camera->GetComponentLocation() - GetActorLocation(); 
	FVector ActorToHand = RightHandController->GetActorLocation() - GetActorLocation();

	// if Camera teleport location is too low such that the character is in the ground, then move the character and knife up
	float PreTeleportCameraDistanceToGround = CustomUtils::DistanceToGround(Camera->GetComponentLocation(), GetWorld());
	float CameraTeleportLocationDistanceToGround = CustomUtils::DistanceToGround(CameraTeleportLocation, GetWorld());
	if (CameraTeleportLocationDistanceToGround < UserEyeHeightCM)
	{
		// Adjust teleport and knife location
		//CameraTeleportLocation.Z = UserEyeHeightCM;
		//CameraTeleportLocationDistanceToGround = UserEyeHeightCM;


	}

	FVector ActorTeleportLocation = KnifeLocation - ActorToHand;
	SetActorLocation(ActorTeleportLocation);
	RightHandController->ForceGrab(TeleportKnife->GetGrabComponent());


	


	// Check for anything blocking teleporting
	// Teleport Character
	// Grab knife

}

void AVRCharacter::TeleGrabLeft()
{

}

void AVRCharacter::PrepTeleGrabLeft()
{
	bInPrepTeleportLeft = true;
	bInPrepTeleportRight = false;
}

void AVRCharacter::PrepTeleGrabRight()
{
	bInPrepTeleportLeft = false;
	bInPrepTeleportRight = true;
}

void AVRCharacter::UnprepTeleGrabLeft()
{
	bInPrepTeleportLeft = false;
}

void AVRCharacter::UnprepTeleGrabRight()
{
	bInPrepTeleportRight = false;
}

FVector AVRCharacter::GetKnifeSpawnLocation()
{
	FVector Offset = Camera->GetForwardVector() * KnifeSpawnDistance;
	return Camera->GetComponentLocation() + Offset;
}

FRotator AVRCharacter::GetKnifeSpawnRotation()
{
	return UKismetMathLibrary::MakeRotationFromAxes(Camera->GetUpVector(), -Camera->GetForwardVector(), -Camera->GetRightVector());
}

