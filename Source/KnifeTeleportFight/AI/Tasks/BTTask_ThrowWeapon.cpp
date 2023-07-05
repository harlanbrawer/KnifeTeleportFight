// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ThrowWeapon.h"
#include "AIController.h"
#include "../../EnemyCharacter.h"

UBTTask_ThrowWeapon::UBTTask_ThrowWeapon()
{
	NodeName = TEXT("Throw Weapon");
}

EBTNodeResult::Type UBTTask_ThrowWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->ThrowRandomlyNearPlayer();

	return EBTNodeResult::Succeeded;
}
