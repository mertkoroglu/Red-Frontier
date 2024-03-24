// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScore.h"

// Sets default values
AGameScore::AGameScore():
	GameScore(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameScore::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameScore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameScore::SetGameScore(float Value)
{
	GameScore += Value;
}

int32 AGameScore::GetGameScore()
{
	return GameScore;
}

