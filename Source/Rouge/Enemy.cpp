// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemySpawner.h"
#include "Collectable.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 1.f;
	Speed = 100.f;
	EnemyPoint = 1;
	Damage = 1.f;
	AttackWait = 1.f;
	ChaseUpdateInterval = 0.1f;
	ChaseUpdateTimer = 0.f;
	bCanAttack = true;
	bReachedPlayer = false;

	SetWalkSpeed(Speed);
	Tags.Add("Enemy");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AI = Cast<AAIController>(GetController());
	ChasePlayer();
}

void AEnemy::DealDamage()
{
	MainCharacter->ReceiveDamage(1.f);
}

void AEnemy::Attack()
{
	TraceStart = GetActorLocation();
	TraceEnd = GetActorLocation() + GetActorForwardVector() * 50.f;

	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(AttackHit, TraceStart, TraceEnd, ECC_Camera, QueryParams);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, .1f, 0, 10.0f);

	if (AttackHit.bBlockingHit && AttackHit.GetActor()->ActorHasTag("Character")) {
		DealDamage();
	}

	bCanAttack = false;
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::ResetAttack, AttackWait, false);
}

void AEnemy::Die()
{
	/*if (EnemySpawner) {
		EnemySpawner->RemoveFromArray(Index);
	}*/
	//MainCharacter->SetGameScore(EnemyPoint);
	GetWorldTimerManager().SetTimer(DieTimer, this, &AEnemy::DestroyEnemy, .5f, false);

	GetWorld()->SpawnActor<ACollectable>(LevelOrb, FVector(GetActorLocation().X - 20.f, GetActorLocation().Y, MainCharacter->GetActorLocation().Z ), GetActorRotation(), FActorSpawnParameters());

	Destroy();
}

void AEnemy::SetEnemySpawner(AEnemySpawner* Ref)
{
	EnemySpawner = Ref;
}

void AEnemy::SetIndex(int32 index)
{
	Index = index;
}

void AEnemy::SetWalkSpeed(float Value)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AEnemy::ResetAttack()
{
	bCanAttack = true;
	CheckCanAttack();
}

void AEnemy::ChasePlayer()
{
	 AI->MoveToLocation(MainCharacter->GetActorLocation(), 50.f, true);
	 DistanceToPlayer = FVector::Distance(MainCharacter->GetActorLocation(), GetActorLocation());
	 if (DistanceToPlayer <= 75.f) {
		 bReachedPlayer = true;
		 CheckCanAttack();
	 }
	 else {
		 bReachedPlayer = false;
	 }

	 GetWorldTimerManager().SetTimer(ChaseTimer, this, &AEnemy::ChasePlayer, 0.1f, false);
}

void AEnemy::CheckCanAttack()
{
	if (bCanAttack && bReachedPlayer)
		Attack();
}

void AEnemy::DestroyEnemy()
{
	Destroy();
}

void AEnemy::MultiplySpeed()
{
	if (MainCharacter->GetGameScore() > 500 && MainCharacter->GetGameScore() < 800) {
		Speed = Speed + Speed * 0.1;
	}
	else if (MainCharacter->GetGameScore() > 800 && MainCharacter->GetGameScore() < 1200) {
		Speed = Speed + Speed * 0.5;
	}
	else if (MainCharacter->GetGameScore() > 1200) {
		Speed = Speed + Speed;
	}

	SetWalkSpeed(Speed);
}

void AEnemy::ReceiveDamage(float Amount)
{
	Health -= Amount;
	if (Health <= 0) {
		Die();
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

