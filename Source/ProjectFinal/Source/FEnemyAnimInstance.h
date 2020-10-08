// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FEnemyAnimInstance.generated.h"

class AFEnemy;

UCLASS()
class PROJECTFINAL_API UFEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "EnemyAnimInstance")
	void UpdateAnimationProperties(float DeltaTime);

	UPROPERTY(BlueprintReadOnly,Category = "EnemyAnimInstance")
	AFEnemy* Owner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyAnimInstance")
	float MovementSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyAnimInstance")
	float Direction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyAnimInstance")
	bool bIsDied;
};
