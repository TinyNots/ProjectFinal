// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnumHeader.h"
#include "FPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFHealthComponent;
class USkeletalMeshComponent;
class UAnimMontage;
class UAnimInstance;
class APlayerCameraManager;
class USoundCue;
class UBoxComponent;
class UCameraShake;
class AFEnemy;
class UParticleSystem;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UBoxComponent* WeaponBoxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	UAnimMontage* HeavyAttackMontage;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	UAnimInstance* AnimInstance;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	APlayerCameraManager* CameraManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	UAnimMontage* HitMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	int CurrentCombo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	bool bIsAttackPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	bool bIsHeavyAttackPressed;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	bool bIsAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Attack")
	TSubclassOf<UDamageType> DamageTypeClass;

	FTimerHandle TimeHandle_SlowTimeDilation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Attack")
	float ToSlowMoDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Attack")
	float SlowMoTimeDilation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Attack")
	float ResetTimeDelay;

	FTimerHandle TimeHandle_ResetTimeDilation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	bool bIsHitStopping;

	UPROPERTY(EditDefaultsOnly, Category = "Player | Attack")
	TSubclassOf<UCameraShake> AttackCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Player | Attack")
	USoundCue* AttackHitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	TArray<AFEnemy*> HitEnemyList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	bool bIsGettingHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Attack")
	UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Attack")
	FName HitSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | ChargeAttack")
	bool bIsCharging;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | ChargeAttack")
	float ChargeTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | ChargeAttack")
	float ChargeDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | ChargeAttack")
	FString ChargeAttackSectionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | ChargeAttack")
	FString ChargeHeavyAttackSectionName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Attack")
	AFEnemy* CombatTarget;

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

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ChangeHitStop(EHitStop HitStopPreset, float NewToSlowMoDelay = 0.0f, float NewSlowMoTimeDilation = 0.0f, float NewResetTimeDelay = 0.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Animation")
	bool bIsInGame;

	UFUNCTION(BlueprintCallable)
	void EnableAttackCollider();

	UFUNCTION(BlueprintCallable)
	void DisableAttackCollider();

	UFUNCTION(BlueprintCallable)
	void ClearHitEnemyList();

	void PlayHitReaction();

	UFUNCTION(BlueprintCallable)
	void EnableMovementInput();

private:

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Attack();

	void AttackCharge();

	void HeavyAttack();

	void HeavyAttackCharge();

	void ToNextAttack();

	void NextAttackCheck();

	void PlayAttackMontage(UAnimMontage* Montage);

	void PlayAttackMontage(UAnimMontage* Montage, FString SectionName);

	void AttackRotationUpdate(float DeltaTime);

	void ResetTimeDilation(AActor* OtherActor);

	void SlowTimeDilation(AActor* OtherActor);

	void StartHitStop(AActor* OtherActor);

	void ChargeAttackTimerUpdate(float DeltaTime);
};
