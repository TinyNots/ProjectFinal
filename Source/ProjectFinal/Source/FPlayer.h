// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFHealthComponent;
class USkeletalMeshComponent;
class UAnimMontage;
class UAnimInstance;

UCLASS()
class PROJECTFINAL_API AFPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UFHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UStaticMeshComponent* WeaponMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	int CurrentCombo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	bool bIsAttackPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	bool bReadyToNextAttack;

	// If next attack boolen is true move to next attack
	UFUNCTION(BlueprintCallable)
	void HandleCombo();

	UFUNCTION(BlueprintCallable)
	void RestartCombo();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Attack")
	float AttackRotationInterpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	bool bStartAttackInterp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	FRotator TargetRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Attack")
	float YawRotationThreshold;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCameraComponent* GetCameraComponent();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Player | Animation")
	float HeadInterpSpeed;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Player | Animation")
	float HeadDegreeThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player | Animation")
	bool bReadyToJump;

	virtual void Jump() override;

private:

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Attack();

	void ToNextAttack();

	void NextAttackCheck();

	void PlayAttackMontage();

	void AttackRotationUpdate(float DeltaTime);
};
