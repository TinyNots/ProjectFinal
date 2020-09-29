// Fill out your copyright notice in the Description page of Project Settings.


#include "FPlayerAnimInstance.h"
#include "FPlayer.h"

void UFPlayerAnimInstance::NativeInitializeAnimation()
{
	if (!Player)
	{
		Player = Cast<AFPlayer>(TryGetPawnOwner());
	}
}

void UFPlayerAnimInstance::UpdateAnimationProperties()
{
	if (Player)
	{
		MovementSpeed = FVector::DotProduct(Player->GetVelocity(), Player->GetActorForwardVector());
		Direction = FVector::DotProduct(Player->GetVelocity(), Player->GetActorRightVector());
	}
}