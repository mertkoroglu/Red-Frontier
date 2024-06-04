// Fill out your copyright notice in the Description page of Project Settings.
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Enemy.h"

// Sets default values
AProjectile::AProjectile():
	BounceAmount(0),
	bBounceable(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionBox) {
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
	if (FireEmitter)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireEmitter, GetActorLocation(), GetActorRotation());
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (OtherActor->ActorHasTag(TEXT("Enemy"))) {
		Enemy = Cast<AEnemy>(OtherActor);
		Enemy->ReceiveDamage(1.f);
	}


	if (bBounceable){
		if (BounceAmount == 1)
			Destroy();
		else
			BounceAmount++;
	}
	else {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireEmitter, Hit.Location, UKismetMathLibrary::MakeRotFromXY(Hit.Normal, Hit.Normal));
		Destroy();
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

