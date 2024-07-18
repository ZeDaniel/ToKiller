// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdatePatrol.h"
#include "ToKillerAiController.h"

UUpdatePatrol::UUpdatePatrol()
{
	NodeName = TEXT("Update Patrol Location");
}

EBTNodeResult::Type UUpdatePatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AToKillerAiController* AiController = Cast<AToKillerAiController>(OwnerComp.GetAIOwner());
	
	if (AiController)
	{
		AiController->UpdatePatrolLocation();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
