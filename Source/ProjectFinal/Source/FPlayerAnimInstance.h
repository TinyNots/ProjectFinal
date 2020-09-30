// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPlayerAnimInstance.generated.h"

class AFPlayer;

UCLASS()
class PROJECTFINAL_API UFPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "PlayerAnimInstance")
	void UpdateAnimationProperties(float DeltaTime);

	UPROPERTY(BlueprintReadOnly, Category = "PlayerAnimInstance")
	AFPlayer* Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnimInstance")
	float MovementSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnimInstance")
	float Direction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnimInstance")
	float LookPitch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnimInstance")
	float LookYaw;

private:

	void HeadRotation(float DeltaTime);
};
