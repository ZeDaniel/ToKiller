// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AimAtPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TOKILLER_API UBTService_AimAtPlayer : public UBTService
{
	GENERATED_BODY()

public:

	UBTService_AimAtPlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
