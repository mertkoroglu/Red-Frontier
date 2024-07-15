// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy.h"
#include "FastEnemy.h"
#include "BaseEnemy.h"
#include "HeavyEnemy.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemySpawner::AEnemySpawner():
	EnemySpawnRate(1.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

    if(NavSys)
	    SpawnEnemy();
}

void AEnemySpawner::SpawnEnemy()
{
    FVector ActorLocation = MainCharacter->GetActorLocation();
    FVector ActorForwardVector = MainCharacter->GetActorForwardVector();

    const float NavigableRadius = 1200.f;

    const float MinDistanceFromCharacterX = 750.f; // Optimal value for this is 450.f, but if we enter the optimum, the points are not goin to be uniform
    const float MinDistanceFromCharacterY = 750.f;

    if (NavSys)
    {
        for (int32 i = 0; i < 100; ++i)
        {
            FVector RandomPoint;
            if (NavSys->K2_GetRandomReachablePointInRadius(GetWorld(), ActorLocation, RandomPoint, NavigableRadius))
            {
                if (
                    FMath::Abs(RandomPoint.X - ActorLocation.X) >= MinDistanceFromCharacterX || FMath::Abs(RandomPoint.Y - ActorLocation.Y) >= MinDistanceFromCharacterY)
                {
                    SpawnLocations.Add(RandomPoint);
                }
            }
        }
    }

    if (SpawnLocations.Num() > 0)
    {
        RandomIndex = FMath::RandRange(1, 15);

        FVector SpawnLocation = SpawnLocations[FMath::RandRange(0, SpawnLocations.Num() - 1)];

        if (RandomIndex >= 1 && RandomIndex <= 7)
        {
            GetWorld()->SpawnActor<AFastEnemy>(FastEnemy, SpawnLocation, FRotator(0.f, 90.f, 0.f), FActorSpawnParameters());
        }
        else if (RandomIndex > 7 && RandomIndex <= 13)
        {
            GetWorld()->SpawnActor<ABaseEnemy>(BaseEnemy, SpawnLocation, FRotator(0.f, 90.f, 0.f), FActorSpawnParameters());
        }
        else
        {
            GetWorld()->SpawnActor<AHeavyEnemy>(HeavyEnemy, SpawnLocation, FRotator(0.f, 90.f, 0.f), FActorSpawnParameters());
        }
    }

    SpawnLocations.Empty();

    WaitForSpawn();
}

void AEnemySpawner::WaitForSpawn()
{
	GetWorldTimerManager().SetTimer(EnemySpawnTimer, this, &AEnemySpawner::SpawnEnemy, EnemySpawnRate, false);
}

void AEnemySpawner::KillAll()
{
	for (int32 i = EnemiesOnLevel.Num() - 1; i >= 0; i--) {
		EnemiesOnLevel[i]->Die();
		EnemiesOnLevel.RemoveAt(i);
	}
}

void AEnemySpawner::RemoveFromArray(int32 index)
{
	EnemiesOnLevel.RemoveAt(index);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::IncreaseSpeedOfGame()
{
	EnemySpawnRate *= .8f;
	UE_LOG(LogTemp, Warning, TEXT("Spawn Speed: %f"), EnemySpawnRate);
}

