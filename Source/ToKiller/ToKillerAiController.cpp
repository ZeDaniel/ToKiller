// Fill out your copyright notice in the Description page of Project Settings.


#include "ToKillerAiController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ToKillerAiCharacter.h"
#include "Tasks\AITask.h"

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

		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());

		UpdatePatrolLocation();
	}
}

void AToKillerAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn && LineOfSightTo(PlayerPawn))
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
		AiPawn->SetChaseTimer(5.f);
	}
	else
	{
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));

		//If last known is present, countdown chase timer
		if (GetBlackboardComponent()->IsVectorValueSet(TEXT("LastKnownPlayerLocation")))
		{
			//Clear last known if ai character has chased for 5 seconds
			if (AiPawn->GetChaseTimer() <= 0)
			{
				FString Text = GetPawn()->GetName();
				UE_LOG(LogTemp, Display, TEXT("Last Know Location for Pawn: %s cleared"), *Text);
				GetBlackboardComponent()->ClearValue(TEXT("LastKnownPlayerLocation"));
			}
			else
			{
				AiPawn->SetChaseTimer(AiPawn->GetChaseTimer() - DeltaTime);
			}
		}
	}	
}
