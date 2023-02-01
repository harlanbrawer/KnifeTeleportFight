// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "Haptics/HapticFeedbackEffect_Base.h"

#include "Components/SceneComponent.h"
#include "GrabComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class KNIFETELEPORTFIGHT_API UGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

	bool TryGrab(class UMotionControllerComponent* GrabbingMotionControllerComponent);
	bool TryRelease(class UMotionControllerComponent* GrabbingMotionControllerComponent);

	bool IsHeld() { return bIsHeld; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Config
	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* HapticEffect;

	// State
	bool bIsHeld = false;
	class UMotionControllerComponent* HoldingControllerComponentRef = nullptr;

	// Utility
	virtual void OnGrabbed() {}
	virtual void OnDropped() {}
};
