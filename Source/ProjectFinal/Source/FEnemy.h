// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FEnemy.generated.h"

class UFHealthComponent;
class UAnimMontage;
class UAnimInstance;
class UBehaviorTree;
class UAIPerceptionComponent;
class UWidgetComponent;

UCLASS()
class PROJECTFINAL_API AFEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	UFHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	UWidgetComponent* HealthWidgetComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	float DestroyLifeSpan;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Dissolve")
	bool bStartDissolve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Dissolve")
	float DissolveValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Dissolve")
	float DissolveTargetValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Dissolve")
	float DissolveInterpSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Dissolve")
	float DissolveDelayTime;

	UFUNCTION(BlueprintCallable)
	void StartDissolve();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Attack")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Enemy | Attack")
	float AttackDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Attack")
	bool bCanKnockBack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Attack")
	float KnockBackPoPitchAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Attack")
	float KnockBackPower;

	APlayerCameraManager* CameraManager;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void Attack();

	UFUNCTION()
	virtual void OnHealthChanged(UFHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Animiation")
	bool bIsDied;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void EnableAttackCollision();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void DisableAttackCollision();

	UFUNCTION()
	virtual	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	void DissolveInterpUpdate(float DeltaTime);

};
