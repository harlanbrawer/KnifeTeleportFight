// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRUI.generated.h"

UCLASS()
class KNIFETELEPORTFIGHT_API AVRUI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRUI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// constants
	UPROPERTY(EditAnywhere)
	FVector UIOffset = FVector(248.63, 0, -26.13);
};
