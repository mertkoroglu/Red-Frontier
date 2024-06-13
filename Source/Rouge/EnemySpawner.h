// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class ROUGE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnEnemy();
	void WaitForSpawn();
private:
	FTimerHandle EnemySpawnTimer;
	float EnemySpawnRate;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABaseEnemy> BaseEnemy;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFastEnemy> FastEnemy;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AHeavyEnemy> HeavyEnemy;

	int8 RandomIndex;
	int8 SpawnOption;

	TArray<class USceneComponent> SceneComps;

	class AMainCharacter* MainCharacter;
	class UNavigationSystemV1* NavSys;

	// TODO: Add Enemy Spawn chance 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemySpawn")
		TArray<FVector> SpawnLocations;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemySpawn")
		TArray<class AEnemy*> EnemiesOnLevel;
	void IncreaseSpeedOfGame();
	void KillAll();
	void RemoveFromArray(int32 index);
};
