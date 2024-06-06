// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile.h"
#include "EnemySpawner.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyGameInstance.h"

// Sets default values
AMainCharacter::AMainCharacter() :
	bFireButtonPressed(false),
	MaxHeath(3.f),
	Health(3.f),
	CharacterSpeed(300.f),
	GunFireSpeed(.5f),
	GameScore(0),
	NextLevelScore(35),
	NextLevelIncrement(35),
	CharacterFireRateLevel(0),
	CharacterSpeedLevel(0),
	CharacterPullRadiusLevel(0),
	bCanCharacterDash(false),
	bCanFire(true),
	CharacterShieldLevel(0),
	CharacterHealthLevel(0),
	CharacterDirectionalShootingLevel(0),
	bCharacterHaveShieldUpgrade(false),
	bCharacterHaveActiveShield(false),
	CharacterShieldTime(0.f),
	bShieldPending(false),
	bBouncyProjectiles(false),
	MaxCharacterFireRateLevel(4),
	MaxCharacterHealthLevel(2),
	MaxCharacterShieldLevel(3),
	MaxCharacterPullRadiusLevel(4),
	TargetFieldOfView(90.f),
	CurrentFieldOfView(90.f),
	MaxCharacterSpeedLevel(3),
	CharacterPullRadius(235.f),
	Tokens(0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CrosshairMesh = CreateDefaultSubobject<UStaticMeshComponent>("CrosshairMesh");
	GetCharacterMovement()->MaxWalkSpeed = CharacterSpeed;

	MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();


	Camera = FindComponentByClass<UCameraComponent>();
	AngleToMouseYaw = FRotator{ 0,0,0 };
	SocketNumber = 1;
	
}

void AMainCharacter::MoveForward(float Value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (Controller != nullptr && Value != 0.0f && !AnimInstance->GetCurrentActiveMontage()) {
		Value = Value / 3;

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator Yaw = { 0.f, Rotation.Yaw, 0.f };

		const FVector Direction = FRotationMatrix{ Yaw }.GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (Controller != nullptr && Value != 0.0f && !AnimInstance->GetCurrentActiveMontage()) {
		Value = Value / 3;

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator Yaw = { 0.f, Rotation.Yaw, 0.f };

		const FVector Direction = FRotationMatrix{ Yaw }.GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::FindMouseHit(float DeltaTime)
{
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FCollisionQueryParams QueryParams;

	GetWorld()->LineTraceSingleByChannel(MouseHit, WorldLocation, WorldDirection * 10000.f + WorldLocation, ECC_Visibility);

	if (MouseHit.bBlockingHit) {
		MouseHitLoc = MouseHit.Location;
		//RobotToMouse = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), MouseHitLoc);

		AngleToMouse = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MouseHitLoc);
		AngleToMouse = FRotator(0, AngleToMouse.Yaw - GetActorRotation().Yaw, 0);
		AngleToMouseYaw.Yaw = AngleToMouse.Yaw;
	}
}

void AMainCharacter::Fire()
{
	SocketName = "GunSocket" + FString::FromInt(SocketNumber);

	if(!bBouncyProjectiles)
		GetWorld()->SpawnActor<AProjectile>(GunProjectile, GetMesh()->GetSocketLocation(FName(SocketName)), GetMesh()->GetSocketRotation(FName(SocketName)), FActorSpawnParameters());
	else
		GetWorld()->SpawnActor<AProjectile>(BouncyGunProjectile, GetMesh()->GetSocketLocation(FName(SocketName)), GetMesh()->GetSocketRotation(FName(SocketName)), FActorSpawnParameters());

	if (SocketNumber == 2) {
		SocketNumber = 1;
	}
	else {
		SocketNumber = 2;
	}

	bCanFire = false;
	GetWorldTimerManager().SetTimer(TimerBetweenShots, this, &AMainCharacter::WaitForFire, GunFireSpeed, false);
	//ZoomFOV();
	UGameplayStatics::PlaySound2D(GetWorld(), FireSound);

	onShoot.Broadcast();
}

void AMainCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	if(bCanFire)
		Fire();
}

void AMainCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AMainCharacter::WaitForFire()
{
	bCanFire = true;
	if(bFireButtonPressed)
		Fire();

}

void AMainCharacter::CheckScore()
{
	if (GameScore - NextLevelScore == 0) {
		Tokens += 1;
		//bUpgradePending = true;
		NextLevelScore += NextLevelIncrement;
		NextLevelIncrement = NextLevelIncrement + 5;
		onUpgradePending.Broadcast();
	}

	if ((GameScore % 24 == 0 && GameScore < 200) || (GameScore % 80 == 0 && GameScore > 800)) {
		EnemySpawnerReference->IncreaseSpeedOfGame();
	}
}

void AMainCharacter::KillAllEnemies()
{
	EnemySpawnerReference->KillAll();
}

void AMainCharacter::Dash()
{
	if (bCanCharacterDash) {
		//const FVector ForwardDir = this->GetActorRotation().Vector();
		LaunchCharacter(GetActorForwardVector() * 1000.f , true, true);
		bCanCharacterDash = false;
		GetWorldTimerManager().SetTimer(DashTimer, this, &AMainCharacter::RefreshDash, 2.f, false);
	}
}

void AMainCharacter::AssignShield()
{
	bCharacterHaveActiveShield = true;
	bShieldPending = false;

	onShieldStateChange.Broadcast();
}

void AMainCharacter::RefreshDash()
{
	bCanCharacterDash = true;
}

void AMainCharacter::IncreaseCharacterSpeed()
{
	switch (CharacterSpeedLevel) {
	case 0:
		CharacterSpeed = 400.f;
		break;
	case 1:
		CharacterSpeed = 500.f;
		break;
	case 2:
		CharacterSpeed = 550.f;
		break;
	default:
		break;
	}

	CharacterSpeedLevel++;

	GetCharacterMovement()->MaxWalkSpeed = CharacterSpeed;
	
}

void AMainCharacter::IncreaseProjectileRate()
{
	switch (CharacterFireRateLevel) {
	case 0:
		GunFireSpeed = .4f;
		break;
	case 1:
		GunFireSpeed = .3f;
		break;
	case 2:
		GunFireSpeed = .2f;
		break;
	case 3:
		GunFireSpeed = .1f;
		break;
	default:
		break;
	}

	CharacterFireRateLevel++;
}

void AMainCharacter::AddDashing()
{
	bCanCharacterDash = true;
}

void AMainCharacter::AddBouncyProjectiles()
{
	bBouncyProjectiles = true;
}

void AMainCharacter::IncreasePullRadius()
{
	switch (CharacterPullRadiusLevel) {
	case 0:
		CharacterPullRadius = 290.f;
		break;
	case 1:
		CharacterPullRadius = 335.f;
		break;
	case 2:
		CharacterPullRadius = 400.f;
		break;
	case 3:
		CharacterPullRadius = 450.f;
		break;
	default:
		break;
	}

	CharacterPullRadiusLevel++;
	onPullRadiusChanged.Broadcast();

}

void AMainCharacter::SetTokens(int32 val)
{
	Tokens -= val;
}

void AMainCharacter::AddShield()
{
	switch (CharacterShieldLevel) {
	case 0:
		AssignShield();
		CharacterShieldTime = 60.f;
		bCharacterHaveShieldUpgrade = true;
		break;
	case 1:
		AssignShield();
		CharacterShieldTime = 40.f;
		break;
	case 2:
		AssignShield();
		CharacterShieldTime = 20.f;
		break;
	default:
		break;
	}

	CharacterShieldLevel++;

}

void AMainCharacter::IncreaseHealth()
{
	switch (CharacterHealthLevel) {
	case 0:
		Health += 1;
		MaxHeath = 4.f;
		break;
	case 1:
		Health += 1;
		MaxHeath = 5.f;
		break;
	default:
		break;
	}

	CharacterHealthLevel++;
	onMaxHealthIncrease.Broadcast();
	onHealthIncrease.Broadcast();
}

void AMainCharacter::AddThreeDirectionShooting()
{
	// TO-DO
}

float AMainCharacter::GetCharacterPullRadius()
{
	return CharacterPullRadius;
}


void AMainCharacter::ReceiveDamage(float Amount)
{
	if (bCharacterHaveActiveShield) {
		bCharacterHaveActiveShield = false;
		onShieldDamaged.Broadcast();
	}
	else {
		Health -= Amount;
		onHealthDecrease.Broadcast();
		if (Health == 0.f) {
			if (MyGameInstance) {
				if (GameScore > MyGameInstance->GetHighScore()) {
					MyGameInstance->SetHighScore(GameScore);
				}
			}
			onDeath.Broadcast();
		}
	}
}

void AMainCharacter::SetGameScore(int Value)
{

	GameScore += Value;
	onGameScoreChange.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), OrbSound);

	CheckScore();
}

int32 AMainCharacter::GetGameScore()
{
	return GameScore;
}


void AMainCharacter::ZoomFOV()
{
	TargetFieldOfView = 88.f;
}

void AMainCharacter::ReturnFOV()
{
	TargetFieldOfView = 90.f;

}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FindMouseHit(DeltaTime);

	if (CrosshairMesh) {
		CrosshairMesh->SetWorldLocation(FVector(MouseHitLoc.X, MouseHitLoc.Y, 1.f));
	}

	if (bCharacterHaveShieldUpgrade && bCharacterHaveActiveShield == false && bShieldPending == false) {
		bShieldPending = true;
		GetWorldTimerManager().SetTimer(ShieldTimer, this, &AMainCharacter::AssignShield, CharacterShieldTime, false);
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction("EKey", IE_Pressed, this, &AMainCharacter::AddDashing);
	PlayerInputComponent->BindAction("SpaceBar", IE_Pressed, this, &AMainCharacter::Dash);
}


