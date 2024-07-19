// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ToKillerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AToKillerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	AToKillerCharacter();

	UFUNCTION(BlueprintCallable)
	bool HasRifle() const { return bHasRifle; }

	UFUNCTION(BlueprintCallable)
	void SetHasRifle(bool NewHasRifle) { bHasRifle = NewHasRifle; }

	/** called when hit by something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, Category = "BodySwap")
	float SwapSpeed = 200.f;
	UPROPERTY(EditAnywhere, Category = "BodySwap")
	float TargetSwapTranslationRadius = 10.f;
	UPROPERTY(EditAnywhere, Category = "BodySwap")
	float TargetSwapRotationLength = 0.01f;
	/** Adjust how much distance affects swap speed. Higher values mean higher distances speed up the swap more. 0 Will nullify speed*/
	UPROPERTY(EditAnywhere, Category = "BodySwap")
	float SwapSpeedDistanceModifier = 1.0f;

private:
	FTransform OnHitTransform;

	bool bHasRifle = false;
	int32 SwapPhase = 0;
	FVector TargetSwapLocation;
	FRotator TargetSwapRotation;
	FRotator LookAtRotation;
	AActor* SwapActor;
	float SwapSpeedDistanceScalar;

	void SwitchBodies(AActor* ActorToSwitchWith);
	void EndSwitchBodies(AActor* ActorToSwitchWith);

	void ProgressSwap(float DeltaTime);

	void SetPlayerEnabledState(bool bPlayerEnabled);

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

