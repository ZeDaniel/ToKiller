// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UpdatePatrol.generated.h"

/**
 * BTTask for updating PatrolLocation to the next value
 */
UCLASS()
class TOKILLER_API UUpdatePatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UUpdatePatrol();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
