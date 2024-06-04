// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

// Sets default values
ACollectable::ACollectable():
	MaxVelocity(600.f),
	Velocity(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	RootComponent = CollisionSphere;

	CollisionSphere->SetSimulatePhysics(true);
	CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	CollisionSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);

	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	InnerSphere = CreateDefaultSubobject<USphereComponent>("InnerSphere");
	InnerSphere->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(InnerSphere);
}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();
	if (CollisionSphere) {
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::OnOverlapBegin);
		CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ACollectable::OnOverlapEnd);
	}
	if (InnerSphere) {
		InnerSphere->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::OnInnerOverlap);
	}

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (MainCharacter) {
		MainCharacter->onPullRadiusChanged.AddDynamic(this, &ACollectable::ChangePullRadius);
		CollisionSphere->SetSphereRadius(MainCharacter->GetCharacterPullRadius());
	}
}

void ACollectable::OnInnerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Character"))) {
		DoSomething(MainCharacter);
		if (PickupSound)
			UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
		Destroy();
	}
}

void ACollectable::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Character"))) {
		if (MainCharacter == nullptr) {
			MainCharacter = Cast<AMainCharacter>(OtherActor);
		}
		bFollowCharacter = true;
		UE_LOG(LogTemp, Warning, TEXT("Overlapping"));
	}
}

void ACollectable::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Character"))) {
		bFollowCharacter = false;
	}
}

void ACollectable::DoSomething(AMainCharacter* Character)
{
	Character->SetGameScore(1);
}

void ACollectable::ChangePullRadius()
{
	if (CollisionSphere && MainCharacter) {
		CollisionSphere->SetSphereRadius(MainCharacter->GetCharacterPullRadius());
		UE_LOG(LogTemp, Warning, TEXT("Changed Pull Radius"));

	}
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFollowCharacter && MainCharacter) {
		Direction = MainCharacter->GetActorLocation() - GetActorLocation();
		Velocity = MaxVelocity / Direction.Size();
		SetActorLocation(GetActorLocation() + Direction * Velocity * DeltaTime);
	}
}

