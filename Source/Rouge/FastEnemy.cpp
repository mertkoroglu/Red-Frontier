// Fill out your copyright notice in the Description page of Project Settings.


#include "FastEnemy.h"

AFastEnemy::AFastEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 1.f;
	Speed = 120.f;
	EnemyPoint = 1;
	Damage = .5f;

	SetWalkSpeed(Speed);
}

void AFastEnemy::BeginPlay()
{
	Super::BeginPlay();
	MultiplySpeed();
}

void AFastEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFastEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
