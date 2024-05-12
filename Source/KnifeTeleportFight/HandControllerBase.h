// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GrabComponent.h"

#include "Engine/EngineTypes.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandControllerBase.generated.h"

UCLASS()
class KNIFETELEPORTFIGHT_API AHandControllerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandControllerBase();

	void SetHand(EControllerHand Hand);

	UFUNCTION(BlueprintCallable)
	void PairController(AHandControllerBase* Controller);

	void Grab();
	void ForceGrab(UGrabComponent* ComponentToGrab);
	void Release();

	class UGrabComponent* GetGrabComponentNearMotionController();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Components
	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* HeldUpDirection;

	// References
	AHandControllerBase* OtherController;

	// Config
	UPROPERTY(EditAnywhere)
	float GrabRadius = 10;

	UPROPERTY(EditAnywhere)
	bool bControllerVisible = true;

	TArray<TEnumAsByte<EObjectTypeQuery>> GrabbableObjectTypes = { UEngineTypes::ConvertToObjectType(UGrabComponent::GrabComponentCollisionChannel) };

	// State
	UGrabComponent* HeldComponent;
};
