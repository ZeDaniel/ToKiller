// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToKillerGameMode.h"
#include "ToKillerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AToKillerGameMode::AToKillerGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PrimaryActorTick.bCanEverTick = true;

}

void AToKillerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGamePaused)
	{
		fLevelTimer += DeltaTime;
	}
}

void AToKillerGameMode::ToggleGamePause()
{
	if (ShouldStartNextLevel)
	{
		LoadNextLevel();
	}

	bGamePaused = !bGamePaused;
	UE_LOG(LogTemp, Display, TEXT("Pause State: %d"), bGamePaused);
	
	UGameplayStatics::SetGamePaused(GetWorld(), bGamePaused);
}

void AToKillerGameMode::QueueNextLevel(FName LevelName)
{
	NextLevel = LevelName;
	ShouldStartNextLevel = true;
}

void AToKillerGameMode::UpdateGunless()
{
	if (Gunless)
		Gunless = false;
}

void AToKillerGameMode::AddPlayerDeath()
{
	PlayerDeaths += 1;
}

void AToKillerGameMode::AddKill()
{
	Kills += 1;
}

void AToKillerGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AToKillerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	HandleStartup();
}

void AToKillerGameMode::HandleStartup()
{
	bGamePaused = true;
	UGameplayStatics::SetGamePaused(GetWorld(), bGamePaused);

	//Bind pause action
	if (APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Pause
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AToKillerGameMode::ToggleGamePause);
			// Restart current level
			EnhancedInputComponent->BindAction(RestartLevelAction, ETriggerEvent::Triggered, this, &AToKillerGameMode::RestartLevel);
		}
	}
}

void AToKillerGameMode::LoadNextLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), NextLevel);
}

void AToKillerGameMode::RestartLevel()
{
	FName LevelToLoad(UGameplayStatics::GetCurrentLevelName(GetWorld()));
	UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad);
}
