// Fill out your copyright notice in the Description page of Project Settings.


#include "ToKillerAiController.h"
#include "Kismet/GameplayStatics.h"

void AToKillerAiController::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	SetFocus(PlayerPawn);

	MoveToActor(PlayerPawn);
}
