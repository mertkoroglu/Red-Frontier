// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameScore.generated.h"

UCLASS()
class ROUGE_API AGameScore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameScore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Score of the Game
	int32 GameScore;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetGameScore(float Value);
	int32 GetGameScore();
};
