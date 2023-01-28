// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportKnife.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ATeleportKnife::ATeleportKnife()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collider"));
	SetRootComponent(DamageCollider);

	ProjectileDamageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Damage Mesh"));
	ProjectileDamageMesh->SetupAttachment(ProjectileMeshContainer);


	//ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	//ProjectileMovementComponent->MaxSpeed = 1300.f;
	//ProjectileMovementComponent->InitialSpeed = 0;
	//ProjectileMovementComponent->bRotationFollowsVelocity = false;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);
}

void ATeleportKnife::Throw(FVector Velocity)
{
	//ProjectileMovementComponent->AddForce(Velocity);

	// Potentially want to interpolate a bit before hard setting this.
	//ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void ATeleportKnife::Recall(FVector Location)
{
	//ProjectileMovementComponent->bRotationFollowsVelocity = false;
	//ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ZeroVector);
	SetActorLocation(Location);
}

// Called when the game starts or when spawned
void ATeleportKnife::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileDamageMesh->OnComponentHit.AddDynamic(this, &ATeleportKnife::OnHit);
}

// Called every frame
void ATeleportKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleportKnife::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& HitResult)
{
}

