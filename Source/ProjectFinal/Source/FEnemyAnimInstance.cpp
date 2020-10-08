// Fill out your copyright notice in the Description page of Project Settings.


#include "FEnemyAnimInstance.h"
#include "FEnemy.h"

void UFEnemyAnimInstance::NativeInitializeAnimation()
{
	if (!Owner)
	{
		Owner = Cast<AFEnemy>(TryGetPawnOwner());
	}

	bIsDied = false;
}

void UFEnemyAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (Owner)
	{
		MovementSpeed = FVector::DotProduct(Owner->GetVelocity(), Owner->GetActorForwardVector());
		Direction = FVector::DotProduct(Owner->GetVelocity(), Owner->GetActorRightVector());
		bIsDied = Owner->bIsDied;
	}
}