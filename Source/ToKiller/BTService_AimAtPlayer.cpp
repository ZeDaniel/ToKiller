// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AimAtPlayer.h"
#include "ToKiller/ToKillerAiController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_AimAtPlayer::UBTService_AimAtPlayer()
{
	NodeName = TEXT("Aim at player if seen");
}

void UBTService_AimAtPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AToKillerAiController* AiController = Cast<AToKillerAiController>(OwnerComp.GetAIOwner());
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (AiController->LineOfSightTo(PlayerPawn))
	{
		AiController->SetFocus(PlayerPawn, EAIFocusPriority::Gameplay);
	}
	else
	{
		AiController->ClearFocus(EAIFocusPriority::Gameplay);
	}
}
