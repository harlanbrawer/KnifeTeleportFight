// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportKnifeProjectileMovement.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GrabComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATeleportKnifeProjectileMovement::ATeleportKnifeProjectileMovement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMeshComponent);

	DamageCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collider"));
	DamageCollider->SetupAttachment(StaticMeshComponent);

	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab Component"));
	GrabComponent->SetupAttachment(StaticMeshComponent);

	GrabCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Grab Collider"));
	GrabCollider->SetupAttachment(GrabComponent);
	GrabCollider->SetCollisionObjectType(UGrabComponent::GrabComponentCollisionChannel);

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(StaticMeshComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->UpdatedComponent = StaticMeshComponent;
}

void ATeleportKnifeProjectileMovement::Recall(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (GrabComponent->IsHeld()) return;

	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
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

	//DamageCollider->OnComponentHit.AddDynamic(this, &ATeleportKnifeProjectileMovement::OnHit);
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ATeleportKnifeProjectileMovement::OnHit);
}

// Called every frame
void ATeleportKnifeProjectileMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ProjectileMovementComponent->UpdatedComponent) {
		if (ProjectileMovementComponent->UpdatedComponent->Mobility) {
			//UE_LOG(LogTemp, Warning, TEXT("log 1: %s"), ProjectileMovementComponent->UpdatedComponent->Mobility != EComponentMobility::Movable ? TEXT("true") : TEXT("false"));
		}
	}
	if (ProjectileMovementComponent->UpdatedComponent) {
		//UE_LOG(LogTemp, Warning, TEXT("log 2: %s"), *ProjectileMovementComponent->UpdatedComponent->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("log 3: %d"), ProjectileMovementComponent->UpdatedComponent->Mobility);
	}

	UE_LOG(LogTemp, Warning, TEXT("should skip update: %s"), ProjectileMovementComponent->ShouldSkipUpdate(DeltaTime) ? TEXT("true") : TEXT("false"));

	UE_LOG(LogTemp, Warning, TEXT("has stopped simulation: %s"), ProjectileMovementComponent->HasStoppedSimulation() ? TEXT("true") : TEXT("false"));

	UE_LOG(LogTemp, Warning, TEXT("is simulation enabled: %s"), ProjectileMovementComponent->bSimulationEnabled ? TEXT("true") : TEXT("false"));

	UE_LOG(LogTemp, Warning, TEXT("is updated component valid: %s"), IsValid(ProjectileMovementComponent->UpdatedComponent) ? TEXT("true") : TEXT("false"));
	
	UE_LOG(LogTemp, Warning, TEXT("max sim iter: %d"), ProjectileMovementComponent->MaxSimulationIterations);

	UE_LOG(LogTemp, Warning, TEXT("only update if rendered: %s"), ProjectileMovementComponent->bUpdateOnlyIfRendered ? TEXT("true") : TEXT("false"));

	// Tack velocity while held since physics is not simulated
	if (!GrabComponent->IsHeld())
	{
		//SetActorRotation(FMath::RInterpTo(
		//	GetActorRotation(),
		//	StaticMeshComponent->GetComponentVelocity().Rotation(),
		//	GetWorld()->GetDeltaSeconds(),
		//	RotationInterpSpeed * GetVelocity().Length()
		//));
		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + StaticMeshComponent->GetComponentVelocity().GetSafeNormal() * 100, 50, FColor::Red, true);
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ProjectileMovementComponent->Velocity.GetSafeNormal() * 100, 50, FColor::Red, true);
	}
}

void ATeleportKnifeProjectileMovement::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("HIT!"));
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;
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

