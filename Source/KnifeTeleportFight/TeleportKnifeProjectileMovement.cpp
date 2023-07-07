// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportKnifeProjectileMovement.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GrabComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATeleportKnifeProjectileMovement::ATeleportKnifeProjectileMovement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collider"));
	SetRootComponent(DamageCollider);
	//DamageCollider->SetSimulatePhysics(true);

	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab Component"));
	GrabComponent->SetupAttachment(DamageCollider);

	GrabCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Grab Collider"));
	GrabCollider->SetupAttachment(GrabComponent);
	//GrabCollider->SetSimulatePhysics(false);
	GrabCollider->SetCollisionObjectType(UGrabComponent::GrabComponentCollisionChannel);

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(DamageCollider);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;
}

void ATeleportKnifeProjectileMovement::Recall(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (GrabComponent->IsHeld()) return;

	DamageCollider->SetSimulatePhysics(false);
	DamageCollider->SetPhysicsLinearVelocity(FVector::ZeroVector);
	SetActorRotation(SpawnRotation);
	SetActorLocation(SpawnLocation);
}

void ATeleportKnifeProjectileMovement::Launch(const FVector& Impulse)
{
	GetGrabComponent()->Launch(Impulse);
}

UGrabComponent* ATeleportKnifeProjectileMovement::GetGrabComponent()
{
	return GrabComponent;
}

// Called when the game starts or when spawned
void ATeleportKnifeProjectileMovement::BeginPlay()
{
	Super::BeginPlay();

	DamageCollider->OnComponentHit.AddDynamic(this, &ATeleportKnifeProjectileMovement::OnHit);
}

// Called every frame
void ATeleportKnifeProjectileMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Always try to point the knife in the direction it is moving
	if (!GrabComponent->IsHeld())
	{
		//SetActorRotation(FMath::RInterpTo(
		//	GetActorForwardVector().Rotation(),
		//	GetVelocity().Rotation(),
		//	GetWorld()->GetDeltaSeconds(),
		//	RotationInterpSpeed * GetVelocity().Length()
		//));
	}
}

void ATeleportKnifeProjectileMovement::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& HitResult)
{
	AActor* MyOwner = GetOwner();
	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner) {
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		//if (HitParticles) {
		//	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		//}
		//if (HitSound) {
		//	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		//}
		//if (HitCameraShakeClass) {
		//	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		//}
	}
}
