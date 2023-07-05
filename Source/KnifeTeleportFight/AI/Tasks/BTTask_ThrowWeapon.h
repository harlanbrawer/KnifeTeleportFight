// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ThrowWeapon.generated.h"

/**
 * 
 */
UCLASS()
class KNIFETELEPORTFIGHT_API UBTTask_ThrowWeapon : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ThrowWeapon();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
