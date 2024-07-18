// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChaseTimer.generated.h"

/**
 * 
 */
UCLASS()
class TOKILLER_API UBTService_ChaseTimer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_ChaseTimer();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
