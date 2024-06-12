// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxHealthIncrease);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDecrease);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthIncrease);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameScoreChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldStateChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpgradePending);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShoot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPullRadiusChanged);


UCLASS()
class ROUGE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	// Function for finding the mouse world location
	void FindMouseHit(float DeltaTime);
	// Gun Fire Function for the character
	void Fire();
	void FireButtonPressed();
	void FireButtonReleased();
	void WaitForFire();
	void CheckScore();
	void KillAllEnemies();
	void Dash();
	void AssignShield();
	void RefreshDash();
	void ZoomFOV();
	void ReturnFOV();

	void SetFlash();
	void StartHitFlash();
	void EndHitFlash();
	void SetDynamicMaterial();

	void SetCameraPP();
	void StartCameraPP();
	void EndCameraPP();
	void Die();
	void PrepareDeath();
private:
	
	UPROPERTY(BlueprintAssignable)
		FOnMaxHealthIncrease onMaxHealthIncrease;
	UPROPERTY(BlueprintAssignable)
		FOnShoot onShoot;
	UPROPERTY(BlueprintAssignable)
		FOnHealthDecrease onHealthDecrease;
	UPROPERTY(BlueprintAssignable)
		FOnHealthIncrease onHealthIncrease;
	UPROPERTY(BlueprintAssignable)
		FOnGameScoreChange onGameScoreChange;
	UPROPERTY(BlueprintAssignable)
		FOnShieldStateChange onShieldStateChange;
	UPROPERTY(BlueprintAssignable)
		FOnShieldDamaged onShieldDamaged;
	UPROPERTY(BlueprintAssignable)
		FOnDeath onDeath;
	UPROPERTY(BlueprintAssignable)
		FOnUpgradePending onUpgradePending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
		class USoundCue* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
		class USoundCue* OrbSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
		class USoundCue* DamageSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
		class USoundCue* DeathSound;

	class UCameraComponent* Camera;

	float CurrentFieldOfView;
	float TargetFieldOfView;


	bool bHitFlash;
	bool bCameraPPActive;
	FTimerHandle HitFlashTimer;
	FTimerHandle CameraPPDeathTimer;
	FTimerHandle CameraPPTimer;
	FTimerHandle DieTimer;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class APlayerController* PlayerController;
	// World Location for line trace
	FVector WorldLocation;
	// World Direction for line trace
	FVector WorldDirection;
	// Line trace hit result for mouse location
	FHitResult MouseHit;
	// Angle Between Mouse and the Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FRotator AngleToMouse;
	// Yaw Angle Between Mouse and the Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FRotator AngleToMouseYaw;
	// Location of the Mouse hit result
	FVector MouseHitLoc; 
	// Mesh of the Crosshair for Mouse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* CrosshairMesh;
	// Projectile class for gun
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AProjectile> GunProjectile;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AProjectile> BouncyGunProjectile;
	// Current socket location for the gun
	int8 SocketNumber;
	// Current socket name for the gun
	FString SocketName;
	// Timer Handle for automatic timed shots
	FTimerHandle TimerBetweenShots;

	FTimerHandle DashTimer;
	// boolean for checkin wheter the fire button is pressed
	bool bFireButtonPressed;
	// Health of the Main Character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		float MaxHeath;
	// Speed of the gun fire
	float GunFireSpeed;

	float CharacterSpeed;
	float CharacterPullRadius;

	bool bCharacterHaveShieldUpgrade;
	bool bCharacterHaveActiveShield;
	// There is a timer asigned for new shield.
	bool bShieldPending;
	bool bDead;
	FTimerHandle ShieldTimer;
	float CharacterShieldTime;

	// Score of the Game. Affects Enemy difficulty percentage and overall game speed.
	int32 GameScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawnerReferences", meta = (AllowPrivateAccess = "true"))
		TSoftObjectPtr<class AEnemySpawner> EnemySpawnerReference;

	// Upgrade Levels For Actions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 CharacterFireRateLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 CharacterSpeedLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		bool bCanCharacterDash;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		bool bBouncyProjectiles;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 CharacterShieldLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 CharacterHealthLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 CharacterDirectionalShootingLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 CharacterPullRadiusLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 MaxCharacterPullRadiusLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 MaxCharacterFireRateLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 MaxCharacterSpeedLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 MaxCharacterShieldLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attributes", meta = (AllowPrivateAccess = "true"))
		int32 MaxCharacterHealthLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget Attributes", meta = (AllowPrivateAccess = "true"))
		int32 NextLevelScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget Attributes", meta = (AllowPrivateAccess = "true"))
		int32 NextLevelIncrement;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Tokens", meta = (AllowPrivateAccess = "true"))
		int32 Tokens;

	bool bCanFire;
	class UMyGameInstance* MyGameInstance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves", meta = (AllowPrivateAccess = "true"))
		class UCurveFloat* HitFlashCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves", meta = (AllowPrivateAccess = "true"))
		class UCurveFloat* CameraPPDeathCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves", meta = (AllowPrivateAccess = "true"))
		class UCurveFloat* CameraPPCurve;

	class UMaterialInterface* Material;
	class UMaterialInstanceDynamic* DynMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Runs when a damage is received
	void ReceiveDamage(float Amount);
	// Updates the GameScore when an enemy is killed.
	void SetGameScore(int Value);
	// Returns the GameScore.
	UFUNCTION(BlueprintCallable)
		int32 GetGameScore();

	// Below are the functions for handling upgrades
	UFUNCTION(BlueprintCallable)
		void IncreaseCharacterSpeed();
	UFUNCTION(BlueprintCallable)
		void IncreaseProjectileRate();
	UFUNCTION(BlueprintCallable)
		void AddShield();
	UFUNCTION(BlueprintCallable)
		void IncreaseHealth();
	UFUNCTION(BlueprintCallable)
		void AddDashing();
	UFUNCTION(BlueprintCallable)
		void AddBouncyProjectiles();
	UFUNCTION(BlueprintCallable)
		void IncreasePullRadius();
	UFUNCTION(BlueprintCallable)
		void SetTokens(int32 val);

	void AddThreeDirectionShooting();


	UPROPERTY(BlueprintAssignable)
		FOnPullRadiusChanged onPullRadiusChanged;
	UFUNCTION(BlueprintCallable)
		float GetCharacterPullRadius();
	UFUNCTION(BlueprintCallable)
		bool GetDeadStatus();
};
