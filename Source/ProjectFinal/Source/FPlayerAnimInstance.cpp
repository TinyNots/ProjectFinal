// Fill out your copyright notice in the Description page of Project Settings.


#include "FPlayerAnimInstance.h"
#include "FPlayer.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFPlayerAnimInstance::NativeInitializeAnimation()
{
	if (!Player)
	{
		Player = Cast<AFPlayer>(TryGetPawnOwner());
	}

	MovementSpeed = 0.0f;
	Direction = 0.0f;
	LookPitch = 0.0f;
	LookYaw = 0.0f;
	bIsJumping = false;
	bIsFalling = false;
}

void UFPlayerAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (Player)
	{
		MovementSpeed = FVector::DotProduct(Player->GetVelocity(), Player->GetActorForwardVector());
		Direction = FVector::DotProduct(Player->GetVelocity(), Player->GetActorRightVector());
		bIsJumping = Player->bWasJumping;
		bIsFalling = Player->GetCharacterMovement()->IsFalling();

		HeadRotation(DeltaTime);
	}
}

void UFPlayerAnimInstance::HeadRotation(float DeltaTime)
{
	FRotator ControlRotation(0.0f);
	AController* PlayerController = Player->GetController();
	if (PlayerController)
	{
		ControlRotation = Player->GetController()->GetControlRotation();
	}

	FRotator ActorRotation = Player->GetActorRotation();
	FRotator Delta = (ControlRotation - ActorRotation).GetNormalized();

	// Return to normal position after the angle reached 135 degrees
	float DotProduct = FVector::DotProduct(Player->GetActorForwardVector(), Player->GetCameraComponent()->GetForwardVector());
	float DegreeAngle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

	if (DegreeAngle > Player->HeadDegreeThreshold)
	{
		Delta = FRotator(0.0f);
	}

	// Lerp to new look target
	FRotator NewRotator(LookPitch, LookYaw, 0.0f);
	NewRotator = FMath::RInterpTo(NewRotator, Delta, DeltaTime, Player->HeadInterpSpeed);

	LookPitch = NewRotator.Pitch;
	LookYaw = NewRotator.Yaw;
}
