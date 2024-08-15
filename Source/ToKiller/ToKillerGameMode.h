// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToKillerGameMode.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), ShowCategories=(Input))
class AToKillerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/** Pause Input Action */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* ContinueAction;
	/** Restart level Input Action */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* PauseAction;

	AToKillerGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ToggleTempGamePause();

	void Continue();

	UFUNCTION(BlueprintCallable)
	void PauseGame();
	UFUNCTION(BlueprintCallable)
	void UnPauseGame();

	UFUNCTION(BlueprintCallable)
	void QueueNextLevel(FName LevelName);

	UFUNCTION(BlueprintCallable)
	bool GamePaused() const { return bGamePaused; }

	UFUNCTION(BlueprintCallable)
	bool GameHasStarted() const { return bGameHasStarted; }

	UFUNCTION(BlueprintCallable)
	void SetGamePaused(bool NewGamePaused) { bGamePaused = NewGamePaused; }

	UFUNCTION(BlueprintCallable)
	float LevelTimer() const { return fLevelTimer; }

	UFUNCTION(BlueprintCallable)
	void RestartLevel();

	void UpdateGunless();

	void AddPlayerDeath();

	void AddKill();

protected:

	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerDeaths = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Kills = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Gunless = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float fLevelTimer = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bGamePaused = false;

	UPROPERTY(EditDefaultsOnly, Category = "Pause")
	TSubclassOf<UUserWidget> PauseWidgetClass;

private:

	class AToKillerCharacter* PlayerCharacter;

	void HandleStartup();

	FName NextLevel;
	bool ShouldStartNextLevel = false;

	void LoadNextLevel();

	UUserWidget* PauseWidget;

	bool bGameHasStarted = false;

	void PauseForStartGame();
};



