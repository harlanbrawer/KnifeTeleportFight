// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RecallWeapon.h"
#include "AIController.h"
#include "../../EnemyCharacter.h"

UBTTask_RecallWeapon::UBTTask_RecallWeapon()
{
	NodeName = TEXT("Throw Weapon");
}

EBTNodeResult::Type UBTTask_RecallWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	Character->RecallWeapon();

	return EBTNodeResult::Succeeded;
}
