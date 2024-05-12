// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportKnife.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GrabComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATeleportKnife::ATeleportKnife()
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
}

void ATeleportKnife::Recall(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (GrabComponent->IsHeld()) return;

	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	SetActorRotation(SpawnRotation);
	SetActorLocation(SpawnLocation);
}

void ATeleportKnife::Launch(const FVector& Impulse)
{
	GetGrabComponent()->Launch(Impulse);
}

UGrabComponent* ATeleportKnife::GetGrabComponent()
{
	return GrabComponent;
}

// Called when the game starts or when spawned
void ATeleportKnife::BeginPlay()
{
	Super::BeginPlay();
	
	//DamageCollider->OnComponentHit.AddDynamic(this, &ATeleportKnife::OnHit);
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ATeleportKnife::OnHit);
}

// Called every frame
void ATeleportKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Point knife in direction of movement if going fast enough
	if (!GrabComponent->IsHeld() && StaticMeshComponent->IsSimulatingPhysics() /* && StaticMeshComponent->GetComponentVelocity().Length() > MINIMUM_VELOCITY_FOR_AUTO_TURNING */)
	{
		float AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), StaticMeshComponent->GetComponentVelocity().GetSafeNormal()));
		FVector AxisToRotateOnToBeStraight = FVector::CrossProduct(GetActorForwardVector(), StaticMeshComponent->GetComponentVelocity()).GetSafeNormal();
		StaticMeshComponent->AddAngularImpulseInRadians(AxisToRotateOnToBeStraight * AngleBetween * StaticMeshComponent->GetComponentVelocity().Length() * 0.01, NAME_None, true);
		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + StaticMeshComponent->GetComponentVelocity().GetSafeNormal() * 100, 10, FColor::Red, false, 5, 0, .1);
	}
}

void ATeleportKnife::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& HitResult)
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
