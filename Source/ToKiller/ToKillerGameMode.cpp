// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToKillerGameMode.h"
#include "ToKillerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UMG.h"

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

	if (bGameHasStarted && !bGamePaused)
	{
		fLevelTimer += DeltaTime;
	}
}

void AToKillerGameMode::ToggleTempGamePause()
{
	if (ShouldStartNextLevel)
	{
		LoadNextLevel();
	}

	bGamePaused = !bGamePaused;
	UE_LOG(LogTemp, Display, TEXT("Pause State: %d"), bGamePaused);
	
	UGameplayStatics::SetGamePaused(GetWorld(), bGamePaused);
}

void AToKillerGameMode::Continue()
{
	if (bGamePaused)
	{
		//set game started if we haven't yet
		if (!bGameHasStarted)
		{
			bGameHasStarted = true;
		}
		ToggleTempGamePause();
	}
}

void AToKillerGameMode::PauseGame()
{
	//ignore pause if game hasn't started (I want to fix the underlying problem, but this will have to do)
	if (!bGameHasStarted)
	{
		return;
	}

	//if we're paused, unpause instead
	if (bGamePaused)
	{
		UnPauseGame();
		return;
	}

	bGamePaused = true;
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	//Create pause widget if template is present
	if (PauseWidgetClass)
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PauseWidget = CreateWidget<UUserWidget>(PlayerController, PauseWidgetClass, TEXT("Pause Widget"));
			PauseWidget->AddToViewport(9999);

			FInputModeGameAndUI PauseInputMode;
			PauseInputMode.SetWidgetToFocus(PauseWidget->GetSlateWidgetFromName(PauseWidget->GetFName()));

			PlayerController->SetInputMode(PauseInputMode);
		}
	}
}

void AToKillerGameMode::UnPauseGame()
{
	//bGamePaused = false;
	//UGameplayStatics::SetGamePaused(GetWorld(), false);

	//Remove pause widget if present
	if (PauseWidget)
	{
		PauseWidget->RemoveFromParent();

		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			FInputModeGameOnly GameInputMode;

			PlayerController->SetInputMode(GameInputMode);
		}
	}
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
	bGamePaused = false;

	//Bind pause action
	if (PlayerCharacter)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()))
		{
			//temp disable input on player until we're ready to play
			PlayerCharacter->DisableInput(PlayerController);
			

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				// Continue game when prompted
				EnhancedInputComponent->BindAction(ContinueAction, ETriggerEvent::Triggered, this, &AToKillerGameMode::Continue);
				// Pause game
				EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AToKillerGameMode::PauseGame);
			}

			DisableInput(PlayerController);
		}
	}

	FTimerHandle StartTimer;
	FTimerDelegate StartTimerDelegate = FTimerDelegate::CreateUObject(this, &AToKillerGameMode::PauseForStartGame);
	GetWorldTimerManager().SetTimer(StartTimer, StartTimerDelegate, 2.f, false);
}

void AToKillerGameMode::LoadNextLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), NextLevel);
}

void AToKillerGameMode::PauseForStartGame()
{
	if (PlayerCharacter)
	{
		APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());

		EnableInput(PlayerController);
		PlayerCharacter->EnableInput(PlayerController);
	}

	ToggleTempGamePause();
}

void AToKillerGameMode::RestartLevel()
{
	if (bGamePaused)
	{
		UnPauseGame();
		Continue();
	}

	FName LevelToLoad(UGameplayStatics::GetCurrentLevelName(GetWorld()));
	UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad);
}
