// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomUtils.h"

CustomUtils::CustomUtils()
{
}

CustomUtils::~CustomUtils()
{
}

float CustomUtils::DistanceToGround(FVector target, UWorld* World)
{
	FVector GroundLocation;
	bool bHit = CustomUtils::TraceVectorToGround(target, World, GroundLocation);
	if (bHit)
	{
		return FVector::Distance(target, GroundLocation);
	}
	else
	{
		return -1;
	}
}

bool CustomUtils::TraceVectorToGround(FVector target, UWorld* World, FVector& OutGroundLocation)
{
	const float GroundTraceMaxDistance = 10000;
	FHitResult HitResult;
	bool bHit = World->LineTraceSingleByChannel(HitResult, target, target + FVector(0, 0, -GroundTraceMaxDistance), ECollisionChannel::ECC_WorldStatic);
	if (!bHit)
	{
		return false;
	}
	else
	{
		OutGroundLocation = HitResult.Location;
		return true;
	}
}
