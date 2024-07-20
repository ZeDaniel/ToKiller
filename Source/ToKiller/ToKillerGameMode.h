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
	class UInputAction* PauseAction;
	/** Restart level Input Action */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* RestartLevelAction;

	AToKillerGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ToggleGamePause();

	UFUNCTION(BlueprintCallable)
	void QueueNextLevel(FName LevelName);

	UFUNCTION(BlueprintCallable)
	bool GamePaused() const { return bGamePaused; }
	UFUNCTION(BlueprintCallable)
	float LevelTimer() const { return fLevelTimer; }

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

private:

	class AToKillerCharacter* PlayerCharacter;

	void HandleStartup();

	FName NextLevel;
	bool ShouldStartNextLevel = false;

	void LoadNextLevel();

	void RestartLevel();
};



