// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	void PairController(AHandControllerBase* Controller);

	void Grab();
	void Release();

	class UGrabComponent* GetGrabComponentNearMotionController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TArray<TEnumAsByte<EObjectTypeQuery>> GrabbableObjectTypes;

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

	// References
	AHandControllerBase* OtherController;

	// Config
	UPROPERTY(EditAnywhere)
	float GrabRadius = 10;

	// State
	UGrabComponent* HeldComponent;
};
