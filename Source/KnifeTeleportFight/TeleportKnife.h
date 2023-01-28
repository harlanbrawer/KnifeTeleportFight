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

	void Throw(FVector Velocity);
	void Recall(FVector Location);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Components
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USceneComponent* ProjectileMeshContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* ProjectileDamageMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	class UBoxComponent* DamageCollider;

	//UPROPERTY(VisibleAnywhere, Category = "Combat")
	//class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystemComponent* TrailParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* HitSound;

	// Utility
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& HitResult);

	// Constants
	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

	// State
	FVector ThrowDirection;
};
