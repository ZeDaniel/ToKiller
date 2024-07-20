// Fill out your copyright notice in the Description page of Project Settings.


#include "ToKillerAiCharacter.h"
#include "TP_WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "ToKillerProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "ToKillerGameMode.h"

// Sets default values
AToKillerAiCharacter::AToKillerAiCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComp = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("Weapon Component"));
	WeaponComp->SetupAttachment(GetMesh(), TEXT("GripPoint"));

}

void AToKillerAiCharacter::HandleDestruction()
{
	SetActorEnableCollision(false);

	if (WeaponDropClass)
	{
		auto WeaponDrop = GetWorld()->SpawnActor<AActor>(WeaponDropClass, GetActorLocation(), GetActorRotation());
	}

	Destroy();
}

void AToKillerAiCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if hit by projectile, initiate body swap
	if (OtherActor && OtherComp && OtherActor->ActorHasTag(TEXT("PlayerProjectile")))
	{
		Cast<AToKillerGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->AddKill();
		Cast<AToKillerProjectile>(OtherActor)->HandleDestruction();
		HandleDestruction();
	}
}

// Called when the game starts or when spawned
void AToKillerAiCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AToKillerAiCharacter::OnHit);		// set up a notification for when this component hits something blocking
}

// Called every frame
void AToKillerAiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AToKillerAiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

