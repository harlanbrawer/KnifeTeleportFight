// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TeleportKnife.h"
#include "BaseCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

/** Direction to throw in */
UENUM()
enum class EThrowDirections : uint8
{
	Straight, Up, Right, Left
};

UCLASS()
class KNIFETELEPORTFIGHT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	virtual void HandleDeath() override;
	void ThrowRandomlyNearPlayer();
	void RecallWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Actions
	//	The NPC starts with the knife in their hand. (Maybe can add spawn and recall animation later)
	//	The NPC moves continuously (may a bit at the player but also moves to dodge flying knives and shuffle back and forth)
	//	The NPC will throw the knife in a random choice of 4 options. At the player, to the left or right of the player, above the player
	//	The NPC will then teleport to the knife and swipe at the player, or if off the ground, attempt to drive strike the player while falling.
	// The NPC should probably pause after each teleport to simulate adjusting to surroundings. Difficulty can scale off of how well the npc dodges, how
	//		long they take to recover and how fast their body moves

	// Config
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATeleportKnife> TeleportKnifeClass; 
	
	// References
	UPROPERTY()
	ATeleportKnife* TeleportKnife;

	// Actions
	void Throw(EThrowDirections Direction);

	// Constants
	UPROPERTY(EditAnywhere)
	float TeleportOffsetHorizontal = 100;
	UPROPERTY(EditAnywhere)
	float TeleportOffsetVertical = 300;
	UPROPERTY(EditAnywhere)
	float ThrowSpeed = 200;

	// State
	bool bIsHolding = false;
};
