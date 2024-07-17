// Fill out your copyright notice in the Description page of Project Settings.


#include "ToKillerAiCharacter.h"
#include "TP_WeaponComponent.h"

// Sets default values
AToKillerAiCharacter::AToKillerAiCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComp = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("Weapon Component"));
	WeaponComp->SetupAttachment(GetMesh(), TEXT("GripPoint"));

}

// Called when the game starts or when spawned
void AToKillerAiCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

