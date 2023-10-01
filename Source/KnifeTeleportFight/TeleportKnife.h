// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportKnife.generated.h"

UCLASS()
class KNIFETELEPORTFIGHT_API ATeleportKnife : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportKnife();

	void Recall(FVector SpawnLocation, FRotator SpawnRotation);
	void Launch(const FVector& Impulse);
	class UGrabComponent* GetGrabComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMeshComponent;

private:
	// Components
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	class UBoxComponent* DamageCollider;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	class UGrabComponent* GrabComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	class UCapsuleComponent* GrabCollider;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystemComponent* TrailParticles;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* HitSound;

	// Config

	// Utility
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& HitResult);

	// Constants
	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

	UPROPERTY(EditAnywhere)
	float RotationInterpSpeed = 0.07f;

	float MINIMUM_VELOCITY_FOR_AUTO_TURNING = 0.1f;
};
