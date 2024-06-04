// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 1.f;
	Speed = 100.f;
	EnemyPoint = 1;
	Damage = 1.f;

	SetWalkSpeed(Speed);
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	MultiplySpeed();
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
