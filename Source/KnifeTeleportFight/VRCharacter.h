// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "HandControllerBase.h"
#include "TeleportKnife.h"
#include "EnhancedInputComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

UCLASS()
class KNIFETELEPORTFIGHT_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY()
	class UPostProcessComponent* PostProcessComponent;

	// Config
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandControllerBase> LeftHandControllerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandControllerBase> RightHandControllerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATeleportKnife> TeleportKnifeClass;

	// References
	UPROPERTY()
	AHandControllerBase* LeftHandController;

	UPROPERTY()
	AHandControllerBase* RightHandController;

	UPROPERTY()
	ATeleportKnife* TeleportKnife;

	// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<class UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* GrabLeftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* GrabRightAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveForwardAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveRightAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* RecallWeaponAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* TeleportAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* PrepTeleGrabRightAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* PrepTeleGrabLeftAction;

	void GrabLeft();
	void ReleaseLeft();
	void GrabRight();
	void ReleaseRight();
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void RecallWeapon();
	void Teleport();
	void TeleGrabLeft();
	void TeleGrabRight();
	void PrepTeleGrabLeft();
	void PrepTeleGrabRight();
	void UnprepTeleGrabLeft();
	void UnprepTeleGrabRight();

	// Constants
	UPROPERTY(EditAnywhere)
	float KnifeSpawnDistance = 30;

	UPROPERTY(EditAnywhere)
	float UserEyeHeightCM = 150;

	// Utility
	FVector GetKnifeSpawnLocation();
	FRotator GetKnifeSpawnRotation();

	// State
	bool bInPrepTeleportLeft = false;
	bool bInPrepTeleportRight = false;
};
