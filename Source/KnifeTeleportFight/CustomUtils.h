// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class KNIFETELEPORTFIGHT_API CustomUtils
{
public:
	CustomUtils();
	~CustomUtils();

	/**
	* Gets the location of the ground underneath the target.
	* Returns false if no ground is found.
	*/
	//static bool GroundLocation(AActor* target, UWorld* World, FVector& OutGroundLocation);
	//static bool GroundLocation(USceneComponent* target, UWorld* World, FVector& OutGroundLocation);
	/**
	* Finds the distance to the ground underneath the target.
	* Returns -1 if no ground is found.
	*/
	//static float DistanceToGround(AActor* target, UWorld* World);
	//static float DistanceToGround(USceneComponent* target, UWorld World);

	static float DistanceToGround(FVector target, UWorld* World);
	static bool TraceVectorToGround(FVector target, UWorld* World, FVector& OutGroundLocation);
};
