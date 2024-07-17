// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ToKillerAiController.generated.h"

/**
 * 
 */
UCLASS()
class TOKILLER_API AToKillerAiController : public AAIController
{
	GENERATED_BODY()

public:
	void UpdatePatrolLocation();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AiBehavior;

private:
	APawn* PlayerPawn;

	class AToKillerAiCharacter* AiPawn;

	int32 CurrentPatrolIndex = 1;
	int32 PatrolIndexIncreaseValue = 1;
	bool IsPatrolReversed = false;

public:
	virtual void Tick(float DeltaTime) override;
	
};
