// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ToKillerAiCharacter.generated.h"

UENUM()
enum class EPatrolType : int32
{
	ONCE = 0		UMETA(DisplayName = "Once"),
	LOOPING = 1		UMETA(DisplayName = "Looping"),
	REVERSING = 2	UMETA(DisplayName = "Reversing")
};

UCLASS()

class TOKILLER_API AToKillerAiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AToKillerAiCharacter();

	TArray<FVector>* GetPatrolLocations() { return &PatrolLocations; }
	EPatrolType GetPatrolType() { return PatrolType; }
	float GetChaseTimer() { return ChaseTimer; }
	void SetChaseTimer(float NewChaseTimer) { ChaseTimer = NewChaseTimer; }

	class UToKiller_TP_WeaponComponent* GetWeaponComp() { return WeaponComp; }

	void HandleDestruction();

	/** called when hit by something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, Category="Weapon", BlueprintReadWrite)
	class UToKiller_TP_WeaponComponent* WeaponComp;

	UPROPERTY(EditAnywhere, Category = "Weapon", BlueprintReadWrite)
	TSubclassOf<AActor> WeaponDropClass;

	/** List of Patrol Locations to move to. 0 should be starting location */
	UPROPERTY(EditAnywhere, Category="Patrol")
	TArray<FVector> PatrolLocations;

	UPROPERTY(EditAnywhere, Category="Patrol")
	EPatrolType PatrolType{ EPatrolType::ONCE };

	UPROPERTY(EditAnywhere, Category="Patrol")
	float ChaseTimer = 5.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
