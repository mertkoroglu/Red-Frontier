// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy.h"
#include "FastEnemy.h"
#include "BaseEnemy.h"
#include "HeavyEnemy.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"

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
	SpawnEnemy();
}

void AEnemySpawner::SpawnEnemy()
{
	// 1 => Bulk of enemies will spawn in front of character
	// 2 => Bulk of Enemies will spawn in ramdom locations on map
	// SpawnOption = FMath::RandRange(1, 2);
	SpawnOption = 2;

	FVector ActorLocation = MainCharacter->GetActorLocation();
	FVector ActorForwardVector = MainCharacter->GetActorForwardVector();

	const float DistanceBetweenPoints = 100.0f; // Adjust this based on your requirements
	const int32 NumPoints = 10;

	/*for (int32 i = 0; i < NumPoints; ++i)
	{
		//FVector PointLocation = ActorLocation + i * DistanceBetweenPoints * ActorForwardVector;
		FVector PointLocation = FVector(ActorLocation.X + ActorForwardVector.Y * i * 40.f + ActorForwardVector.X * 550.f, ActorLocation.Y + ActorForwardVector.X * i * 40.f + ActorForwardVector.Y * 1100.f, 33.f);


		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Point %d: %s"), i, *PointLocation.ToString()));
			DrawDebugPoint(GetWorld(), PointLocation, 10.0f, FColor::Green, false, 5.0f);
		}
	}*/

	if (SpawnOption == 1) {
		
	}
	else {
		for (float i = 1300.f; i > -1300.f; i -= 100) {
			SpawnLocations.Add(FVector(MainCharacter->GetActorLocation().X + 550.f, MainCharacter->GetActorLocation().Y + i, 33.f));
			SpawnLocations.Add(FVector(MainCharacter->GetActorLocation().X + -550.f, MainCharacter->GetActorLocation().Y + i, 33.f));
		}

		for (float i = 550.f; i > -550.f; i -= 60) {
			SpawnLocations.Add(FVector(MainCharacter->GetActorLocation().X + i, MainCharacter->GetActorLocation().Y + 1300.f, 33.f));
			SpawnLocations.Add(FVector(MainCharacter->GetActorLocation().X + i, MainCharacter->GetActorLocation().Y + -1300.f, 33.f));
		}
	}

	

	RandomIndex = FMath::RandRange(1, 15); 


	if (RandomIndex >= 1 && RandomIndex <= 7) {
		GetWorld()->SpawnActor<AFastEnemy>(FastEnemy, SpawnLocations[FMath::RandRange(0, SpawnLocations.Num() - 1)], FRotator(0.f, 90.f, 0.f), FActorSpawnParameters());
	}else if(RandomIndex > 7 && RandomIndex <= 13) {
		GetWorld()->SpawnActor<ABaseEnemy>(BaseEnemy, SpawnLocations[FMath::RandRange(0, SpawnLocations.Num() - 1)], FRotator(0.f, 90.f, 0.f), FActorSpawnParameters());
	}
	else {
		GetWorld()->SpawnActor<AHeavyEnemy>(HeavyEnemy, SpawnLocations[FMath::RandRange(0, SpawnLocations.Num() - 1)], FRotator(0.f, 90.f, 0.f), FActorSpawnParameters());
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

