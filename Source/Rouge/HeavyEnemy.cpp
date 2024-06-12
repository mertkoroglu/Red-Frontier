// Fill out your copyright notice in the Description page of Project Settings.


#include "HeavyEnemy.h"


AHeavyEnemy::AHeavyEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 3.f;
	Speed = 65.f;
	EnemyPoint = 1;
	Damage = 3.f;
	TraceLength = 110.f;
	TargetDistanceToCharacter = 110.f;

	SetWalkSpeed(Speed);
}

void AHeavyEnemy::BeginPlay()
{
	Super::BeginPlay();
	MultiplySpeed();
}

void AHeavyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHeavyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
