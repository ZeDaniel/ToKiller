// Fill out your copyright notice in the Description page of Project Settings.


#include "ToKillerAiController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ToKillerAiCharacter.h"
#include "Tasks\AITask.h"
#include "ToKillerCharacter.h"
#include "Camera/CameraComponent.h"

void AToKillerAiController::UpdatePatrolLocation()
{
	//Get character containing the patrols
	AToKillerAiCharacter* AiCharacter = Cast<AToKillerAiCharacter>(GetPawn());
	if (AiCharacter)
	{
		//Get patrollocation array
		TArray<FVector>* PatrolLocations = AiCharacter->GetPatrolLocations();
		if (PatrolLocations->Num() > 1)
		{
			//If character reached either end of the patrols, update the index
			if (PatrolLocations->Num() <= CurrentPatrolIndex || CurrentPatrolIndex < 0)
			{
				if(AiCharacter->GetPatrolType() == EPatrolType::LOOPING)
				{
					CurrentPatrolIndex = 0;

					GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation"), (*PatrolLocations)[CurrentPatrolIndex]);
					CurrentPatrolIndex += PatrolIndexIncreaseValue;
				}
				else if (AiCharacter->GetPatrolType() == EPatrolType::REVERSING)
				{
					IsPatrolReversed = !IsPatrolReversed;
					PatrolIndexIncreaseValue = -PatrolIndexIncreaseValue;
					CurrentPatrolIndex += PatrolIndexIncreaseValue*2;

					GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation"), (*PatrolLocations)[CurrentPatrolIndex]);
					CurrentPatrolIndex += PatrolIndexIncreaseValue;
				}
			}
			else
			{
				GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation"), (*PatrolLocations)[CurrentPatrolIndex]);
				CurrentPatrolIndex += PatrolIndexIncreaseValue;
			}
		}
	}
}

void AToKillerAiController::BeginPlay()
{
	Super::BeginPlay();

	AiPawn = Cast<AToKillerAiCharacter>(GetPawn());

	if (AiBehavior)
	{
		RunBehaviorTree(AiBehavior);

		UpdatePatrolLocation();
	}
}

FVector AToKillerAiController::GetFocalPointOnActor(const AActor* Actor) const
{
	if (const AToKillerCharacter* Player = Cast<AToKillerCharacter>(Actor))
	{
		return Player->GetFirstPersonCameraComponent()->GetComponentLocation();
	}
	else
	{
		return Super::GetFocalPointOnActor(Actor);
	}

	return FVector();
}

void AToKillerAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
