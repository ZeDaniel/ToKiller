// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ToKillerAiController.generated.h"

/**
 * 
 */
UCLASS()
class TOKILLER_API AToKillerAiController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};
