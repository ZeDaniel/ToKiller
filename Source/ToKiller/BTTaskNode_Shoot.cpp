// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Shoot.h"
#include "ToKillerAiController.h"
#include "ToKillerAiCharacter.h"
#include "ToKiller_TP_WeaponComponent.h"


UBTTaskNode_Shoot::UBTTaskNode_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTaskNode_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AToKillerAiController* AiController = Cast<AToKillerAiController>(OwnerComp.GetAIOwner());

	if (AiController)
	{
		AToKillerAiCharacter* AiCharacter = Cast<AToKillerAiCharacter>(AiController->GetCharacter());
		if (AiCharacter)
		{
			AiCharacter->GetWeaponComp()->Fire();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
