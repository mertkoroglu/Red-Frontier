// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


UCLASS()
class ROUGE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DealDamage();
	void Attack();
	void SetWalkSpeed(float Value);
	void ResetAttack();
	void ChasePlayer();
	void CheckCanAttack();
	void DestroyEnemy();
	void MultiplySpeed();
	void SetFlash();
	void StartHitFlash();
	void EndHitFlash();
	void SetDynamicMaterial();

	// Health of the Enemy
	float Health;
	// Speed of the enemy.
	float Speed;
	// Defines how much the enemy affects the score. More difficult to kill, more points. 
	int32 EnemyPoint;
	// Damage amount of the enemy
	float Damage;
	float TraceLength;
	float TargetDistanceToCharacter;
private:

	// Hit result of attack action
	FHitResult AttackHit;
	// Start point of attack
	FVector TraceStart;
	// End point of attack
	FVector TraceEnd;
	FCollisionQueryParams QueryParams;
	// MainCharacter object
	class AMainCharacter* MainCharacter;
	class AAIController* AI;
	class AEnemySpawner* EnemySpawner;
	int32 Index;

	FTimerHandle AttackTimer;
	FTimerHandle ChaseTimer;
	FTimerHandle DieTimer;

	float AttackWait;
	bool bCanAttack;
	float ChaseUpdateInterval;
	float ChaseUpdateTimer;
	float DistanceToPlayer;
	bool bReachedPlayer;
	bool bHitFlash;
	bool bDead;
	FTimerHandle HitFlashTimer;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collectables", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ACollectable> LevelOrb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves", meta = (AllowPrivateAccess = "true"))
		class UCurveFloat* HitFlashCurve;

	class UMaterialInterface* Material;
	class UMaterialInstanceDynamic* DynMaterial;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackMontage;

	class UAnimInstance* AnimInstance; 


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ReceiveDamage(float Amount);
	void Die();
	void SetEnemySpawner(AEnemySpawner*);
	void SetIndex(int32 index);
};
