// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChaseTimer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_ChaseTimer::UBTService_ChaseTimer()
{
	NodeName = TEXT("Update Chase Timer");
}

void UBTService_ChaseTimer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	float ChaseTimer = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());
	ChaseTimer += DeltaSeconds;
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), ChaseTimer);
}
