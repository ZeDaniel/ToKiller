// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToKillerCharacter.h"
#include "ToKillerProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "ToKillerPlayerController.h"
#include "ToKillerAiCharacter.h"
#include "ToKillerAiController.h"
#include "BrainComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AToKillerCharacter

AToKillerCharacter::AToKillerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AToKillerCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnHitTransform = GetActorTransform();

	//if hit by projectile, initiate body swap && destroy projectile
	if (OtherActor && OtherComp && OtherActor->ActorHasTag(TEXT("Projectile")))
	{
		SwapActor = OtherActor->GetOwner();
		Cast<AToKillerProjectile>(OtherActor)->HandleDestruction();
		SwitchBodies(SwapActor);
	}
}

void AToKillerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SwapPhase > 0)
	{
		ProgressSwap(DeltaTime);
	}
}

void AToKillerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AToKillerCharacter::OnHit);		// set up a notification for when this component hits something blocking
}

void AToKillerCharacter::SwitchBodies(AActor* ActorToSwitchWith)
{
	if (AToKillerAiCharacter* AiCharacter = Cast<AToKillerAiCharacter>(ActorToSwitchWith))
	{
		//Slow down everything except player. Disable physics on player for now
		SetPlayerEnabledState(false);
		SetActorTransform(OnHitTransform);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
		CustomTimeDilation = 10.f;

		//Disable collision of actor we're switching with
		AiCharacter->SetActorEnableCollision(false);
		AToKillerAiController* AiController = Cast<AToKillerAiController>(AiCharacter->GetController());
		if (AiController)
		{
			AiController->GetBrainComponent()->StopLogic(TEXT("Swapping"));
		}

		//Gather details for sending us to the actor's location
		TargetSwapLocation = ActorToSwitchWith->GetActorLocation();
		TargetSwapLocation.Z += 5;
		LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetSwapLocation);
		TargetSwapRotation = LookAtRotation;
		TargetSwapRotation.Yaw += 180;
		SwapSpeedDistanceScalar = ((GetActorLocation() - TargetSwapLocation).Length() / 500) * SwapSpeedDistanceModifier;
		SwapSpeedDistanceScalar = FMath::Max(SwapSpeedDistanceScalar, 1);

		//Begin player swap movement
		SwapPhase = 1;
	}
}

void AToKillerCharacter::EndSwitchBodies(AActor* ActorToSwitchWith)
{
	if (AToKillerAiCharacter* AiCharacter = Cast<AToKillerAiCharacter>(ActorToSwitchWith))
	{
		AiCharacter->HandleDestruction();
	}

	//Unslow the game simulation
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	CustomTimeDilation = 1.f;

	//Re-enable player
	SetPlayerEnabledState(true);
}

void AToKillerCharacter::ProgressSwap(float DeltaTime)
{
	if (SwapPhase == 1)
	{
		//Initial rotation to face target
		GetController()->SetControlRotation(FMath::RInterpConstantTo(GetControlRotation(), LookAtRotation, DeltaTime, SwapSpeed * SwapSpeedDistanceScalar));
		if ((GetControlRotation().GetNormalized().Yaw - LookAtRotation.GetNormalized().Yaw) < TargetSwapRotationLength && (GetControlRotation().GetNormalized().Yaw - LookAtRotation.GetNormalized().Yaw) > -TargetSwapRotationLength)
		{
			SwapPhase = 2;
		}
	}
	else if (SwapPhase == 2)
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), TargetSwapLocation, DeltaTime, SwapSpeed * SwapSpeedDistanceScalar));

		if ((GetActorLocation() - TargetSwapLocation).Length() < TargetSwapTranslationRadius * 2)
		{
			SwapPhase = 3;
		}
	}
	else if (SwapPhase == 3)
	{
		if ((GetActorLocation() - TargetSwapLocation).Length() > TargetSwapTranslationRadius)
		{
			SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), TargetSwapLocation, DeltaTime, SwapSpeed * SwapSpeedDistanceScalar));
		}
		GetController()->SetControlRotation(FMath::RInterpConstantTo(GetControlRotation(), TargetSwapRotation, DeltaTime, SwapSpeed * 1.25));

		/*UE_LOG(LogTemp, Display, TEXT("Current Player Rotation Yaw: %f"), GetControlRotation().GetNormalized().Yaw);
		UE_LOG(LogTemp, Display, TEXT("Current Target Rotation Yaw: %f"), TargetSwapRotation.GetNormalized().Yaw);
		float YawDifference = (GetControlRotation().GetNormalized().Yaw - TargetSwapRotation.GetNormalized().Yaw);
		UE_LOG(LogTemp, Display, TEXT("Difference of Yaws: %f"), YawDifference);*/

		//If rotation and location are close enough to target, end swap
		if (((GetControlRotation().GetNormalized().Yaw - TargetSwapRotation.GetNormalized().Yaw) < TargetSwapRotationLength) 
			&& ((GetControlRotation().GetNormalized().Yaw - TargetSwapRotation.GetNormalized().Yaw) > -TargetSwapRotationLength)
			&& ((GetActorLocation() - TargetSwapLocation).Length() < TargetSwapTranslationRadius))
		{
			SwapPhase = 0;
			EndSwitchBodies(SwapActor);
		}
	}
}

void AToKillerCharacter::SetPlayerEnabledState(bool bPlayerEnabled)
{
	/*GetCapsuleComponent()->SetSimulatePhysics(bPlayerEnabled);
	GetCapsuleComponent()->SetEnableGravity(bPlayerEnabled);
	GetMesh1P()->SetEnableGravity(bPlayerEnabled);
	GetMesh()->SetEnableGravity(bPlayerEnabled);*/
	SetActorEnableCollision(bPlayerEnabled);

	if (bPlayerEnabled)
	{
		GetMovementComponent()->Activate();
		EnableInput(Cast<AToKillerPlayerController>(GetController()));
	}
	else
	{
		DisableInput(Cast<AToKillerPlayerController>(GetController()));
		GetMovementComponent()->Deactivate();
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AToKillerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AToKillerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AToKillerCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AToKillerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AToKillerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}