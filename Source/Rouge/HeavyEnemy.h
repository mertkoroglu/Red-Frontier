// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "HeavyEnemy.generated.h"

/**
 *
 */
UCLASS()
class ROUGE_API AHeavyEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHeavyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
