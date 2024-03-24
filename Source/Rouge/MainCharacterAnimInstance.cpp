// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"

void UMainCharacterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (MainCharacter == nullptr) {
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	}
	else {
		FVector Velocity{ MainCharacter->GetVelocity() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
	}
}

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}
