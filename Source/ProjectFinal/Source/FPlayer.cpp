// Fill out your copyright notice in the Description page of Project Settings.


#include "FPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "FHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "ProjectFinal/ProjectFinal.h"
#include "FEnemy.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "FPlayerController.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
AFPlayer::AFPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponSocketName = "hand_rSocket";

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(GetRootComponent());

	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 20.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<UFHealthComponent>(TEXT("HealthComp"));

	// Weapon Collision Setup
	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComp"));
	WeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMeshComp->SetupAttachment(GetMesh(), WeaponSocketName);

	WeaponBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBoxComp"));
	WeaponBoxComp->SetCollisionObjectType(COLLISION_WEAPON);
	WeaponBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponBoxComp->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Overlap);
	WeaponBoxComp->SetupAttachment(WeaponMeshComp, WeaponSocketName);

	bUseControllerRotationYaw = false;
	UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement();
	if (CharacterMovementComp)
	{
		CharacterMovementComp->bOrientRotationToMovement = true;
		CharacterMovementComp->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	}

	CurrentCombo = 0;
	bIsAttackPressed = false;
	bIsHeavyAttackPressed = false;
	bReadyToNextAttack = false;
	bIsCharging = false;
	TargetDistanceLimit = 500.0f;

	// Attack Rotation Init
	AttackRotationInterpSpeed = 10.0f;
	bStartAttackInterp = false;
	TargetRotation = FRotator(0.0f);
	YawRotationThreshold = 3.0f;
	bIsAttacking = false;

	// Hitstop Init
	ToSlowMoDelay = 0.015f;			// Light Hit = 0.015f;	Heavy = ?	// New Light Hit = 0.015f;
	SlowMoTimeDilation = 0.3f;		// Light Hit = 0.5f;	Heavy = ?	// New Light Hit = 0.3f;
	ResetTimeDelay = 0.075f;		// Light Hit = 0.05f;	Heavy = ?	// New Light Hit = 0.075f;
	bIsHitStopping = false;

	// Animation Init
	HeadInterpSpeed = 3.0f;
	HeadDegreeThreshold = 120.0f;
	bReadyToJump = true;
	bIsInGame = false;
}

// Called when the game starts or when spawned
void AFPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Set camera pitch rotation
	if (GetWorld())
	{
		CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		if (CameraManager)
		{
			CameraManager->ViewPitchMin = -45.0f;
			CameraManager->ViewPitchMax = 45.0f;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("There's problem with your CameraManager"));
		}
	}

	AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("There's problem with your AnimInstace"));
	}

	WeaponBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPlayer::CombatOnOverlapBegin);

	bIsInGame = true;
}

void AFPlayer::HandleCombo()
{
	bReadyToNextAttack = true;
	DisableAttackCollider();
}

void AFPlayer::RestartCombo()
{
	CurrentCombo = 0;
	bReadyToNextAttack = false;
	bIsAttacking = false;
	bIsAttackPressed = false;
	bIsHeavyAttackPressed = false;
}

// Called every frame
void AFPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	NextAttackCheck();

	AttackRotationUpdate(DeltaTime);

	ChargeAttackTimerUpdate(DeltaTime);

	// Debug
	if (CombatTarget)
	{
		float Distance = FVector::Dist(GetActorLocation(), CombatTarget->GetActorLocation());
		if (Distance > TargetDistanceLimit || CombatTarget->bIsDied)
		{
			CombatTarget = nullptr;
		}
	}

	bPressedJump = false;
}

// Called to bind functionality to input
void AFPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPlayer::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AFPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AFPlayer::AddControllerYawInput);

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &AFPlayer::Attack);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Released, this, &AFPlayer::AttackCharge);
	PlayerInputComponent->BindAction("HeavyAttack", EInputEvent::IE_Pressed, this, &AFPlayer::HeavyAttack);
	PlayerInputComponent->BindAction("HeavyAttack", EInputEvent::IE_Released, this, &AFPlayer::HeavyAttackCharge);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AFPlayer::Jump);
}

UCameraComponent* AFPlayer::GetCameraComponent()
{
	return CameraComp;
}

void AFPlayer::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFEnemy* Enemy = Cast<AFEnemy>(OtherActor);
	if (Enemy && DamageTypeClass)
	{
		// Debug
		if (!CombatTarget || CombatTarget != Enemy)
		{
			CombatTarget = Enemy;
		}

		if (HitEnemyList.Num() != 0)
		{
			for (auto & HitEnemy : HitEnemyList)
			{
				if (HitEnemy == Enemy)
				{
					return;
				}
			}
		}

		HitEnemyList.AddUnique(Enemy);

		UGameplayStatics::ApplyDamage(Enemy, 10.0f, GetController(), this, DamageTypeClass);

		StartHitStop(Enemy);

		if (AttackHitSound && HitParticle)
		{
			FVector HitLocation = GetMesh()->GetSocketByName(HitSocketName)->GetSocketLocation(GetMesh());
			FVector SpawnLocation = OtherActor->GetActorLocation();
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), (USoundBase*)AttackHitSound, HitLocation);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitLocation);
		}
	}
}

void AFPlayer::ChangeHitStop(EHitStop HitStopPreset, float NewToSlowMoDelay, float NewSlowMoTimeDilation, float NewResetTimeDelay)
{
	switch (HitStopPreset)
	{
	case EHitStop::Light:
		ToSlowMoDelay = 0.015f;
		SlowMoTimeDilation = 0.5f;
		ResetTimeDelay = 0.075f;
		break;
	case EHitStop::Medium:
		UE_LOG(LogTemp,Warning,TEXT("Hit Stop Medium Preset haven't set yet."))
		break;
	case EHitStop::Heavy:
		ToSlowMoDelay = 0.015f;
		SlowMoTimeDilation = 0.1f;
		ResetTimeDelay = 0.1f;
		break;
	case EHitStop::Custom:
		ToSlowMoDelay = NewToSlowMoDelay;
		SlowMoTimeDilation = NewSlowMoTimeDilation;
		ResetTimeDelay = NewResetTimeDelay;
		break;
	}
}

void AFPlayer::EnableAttackCollider()
{
	WeaponBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AFPlayer::DisableAttackCollider()
{
	WeaponBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFPlayer::ClearHitEnemyList()
{
	HitEnemyList.Empty();
}

void AFPlayer::PlayHitReaction()
{
	if (AnimInstance && HitMontage)
	{
		bIsAttacking = false;
		CurrentCombo = 0;

		bReadyToJump = true;
		bIsGettingHit = true;

		int Random = FMath::RandRange(1, 2);

		FString StringHitName = "Hit0";
		StringHitName.AppendInt(Random);
		FName HitSectionName = FName(*StringHitName);

		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(HitSectionName, HitMontage);
	}
}

void AFPlayer::EnableMovementInput()
{
	bIsGettingHit = false;
}

void AFPlayer::MoveForward(float Value)
{
	if (!bIsGettingHit)
	{
		AddMovementInput(CameraComp->GetForwardVector() * Value);
	}
}

void AFPlayer::MoveRight(float Value)
{
	if (!bIsGettingHit)
	{
		AddMovementInput(CameraComp->GetRightVector() * Value);
	}
}

void AFPlayer::Attack()
{
	// debug
	bIsCharging = true;

	bIsAttackPressed = true;
	if (AnimInstance && AttackMontage && !GetCharacterMovement()->IsFalling() && !bIsGettingHit)
	{
		if (AnimInstance->Montage_IsPlaying(AttackMontage) || bIsAttacking)
		{
			return;
		}

		bIsAttackPressed = false;
		bIsAttacking = true;
		CurrentCombo++;

		HitEnemyList.Empty();

		// Start Interp To Last Input Direction
		bStartAttackInterp = true;
		TargetRotation = GetLastMovementInputVector().Rotation();

		PlayAttackMontage(AttackMontage);
	}
}

void AFPlayer::AttackCharge()
{
	bIsCharging = false;
	if (ChargeTimer >= ChargeDelay)
	{
		bIsAttackPressed = false;
		bIsAttacking = true;

		HitEnemyList.Empty();

		bStartAttackInterp = true;
		TargetRotation = GetLastMovementInputVector().Rotation();

		ChangeHitStop(EHitStop::Custom, 0.1f, 0.025f, 0.1f);
		PlayAttackMontage(AttackMontage, ChargeAttackSectionName);
	}
	ChargeTimer = 0.0f;
}

void AFPlayer::HeavyAttack()
{
	// debug
	bIsCharging = true;

	bIsHeavyAttackPressed = true;
	if (AnimInstance && HeavyAttackMontage && !GetCharacterMovement()->IsFalling() && !bIsGettingHit)
	{
		if (AnimInstance->Montage_IsPlaying(HeavyAttackMontage) || bIsAttacking)
		{
			return;
		}

		bIsHeavyAttackPressed = false;
		bIsAttacking = true;
		CurrentCombo++;

		HitEnemyList.Empty();

		// Start Interp To Last Input Direction
		bStartAttackInterp = true;
		TargetRotation = GetLastMovementInputVector().Rotation();

		PlayAttackMontage(HeavyAttackMontage);
	}
}

void AFPlayer::HeavyAttackCharge()
{
	bIsCharging = false;
	if (ChargeTimer >= ChargeDelay)
	{
		bIsHeavyAttackPressed = false;
		bIsAttacking = true;

		HitEnemyList.Empty();

		bStartAttackInterp = true;
		TargetRotation = GetLastMovementInputVector().Rotation();

		ChangeHitStop(EHitStop::Custom, 0.1f, 0.025f, 0.1f);
		PlayAttackMontage(HeavyAttackMontage, ChargeHeavyAttackSectionName);
	}
	ChargeTimer = 0.0f;
}

void AFPlayer::ToNextAttack()
{
	if (!AnimInstance || !AttackMontage || !HeavyAttackMontage)
	{
		return;
	}

	bIsAttackPressed = false;
	bIsHeavyAttackPressed = false;
	bReadyToNextAttack = false;
	CurrentCombo++;

	HitEnemyList.Empty();

	// Start Interp To Last Input Direction
	bStartAttackInterp = true;
	TargetRotation = GetLastMovementInputVector().Rotation();

	AnimInstance->Montage_Stop(0.1f, AnimInstance->GetCurrentActiveMontage());

	//PlayAttackMontage(HeavyAttackMontage);
}

void AFPlayer::NextAttackCheck()
{
	if (bReadyToNextAttack)
	{
		if (bIsAttackPressed)
		{
			ToNextAttack();
			PlayAttackMontage(AttackMontage);
		}

		if (bIsHeavyAttackPressed)
		{
			ToNextAttack();
			PlayAttackMontage(HeavyAttackMontage);
		}
	}
}

void AFPlayer::PlayAttackMontage(UAnimMontage* Montage)
{
	FString SectionName = "Combo0";
	if (Montage == HeavyAttackMontage)
	{
		SectionName = "HeavyCombo0";
	}

	SectionName.AppendInt(CurrentCombo);
	
	PlayAttackMontage(Montage, SectionName);
}

void AFPlayer::PlayAttackMontage(UAnimMontage* Montage, FString SectionName)
{
	FName ComboSectionName = FName(*SectionName);
	AnimInstance->Montage_Play(Montage);
	AnimInstance->Montage_JumpToSection(ComboSectionName, Montage);
}

void AFPlayer::AttackRotationUpdate(float DeltaTime)
{
	if (bStartAttackInterp)
	{
		if (TargetRotation == FRotator::ZeroRotator)
		{
			if (CombatTarget)
			{
				TargetRotation = (CombatTarget->GetActorLocation() - GetActorLocation()).Rotation();
			}
			else
			{
				bStartAttackInterp = false;
				return;
			}
		}

		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, AttackRotationInterpSpeed);
		NewRotation.Pitch = GetActorRotation().Pitch;
		SetActorRotation(NewRotation);

		if (NewRotation.Yaw >= TargetRotation.Yaw - YawRotationThreshold &&
			NewRotation.Yaw <= TargetRotation.Yaw + YawRotationThreshold)
		{
			bStartAttackInterp = false;
		}
	}
}

void AFPlayer::ResetTimeDilation(AActor* OtherActor)
{
	CustomTimeDilation = 1.0f;
	OtherActor->CustomTimeDilation = 1.0f;
	bIsHitStopping = false;
}

void AFPlayer::SlowTimeDilation(AActor* OtherActor)
{
	CustomTimeDilation = SlowMoTimeDilation;
	OtherActor->CustomTimeDilation = SlowMoTimeDilation;

	if (ResetTimeDelay > 0.0f)
	{
		FTimerDelegate ResetTimerDel;
		ResetTimerDel.BindUObject(this, &AFPlayer::ResetTimeDilation, OtherActor);

		GetWorldTimerManager().SetTimer(TimeHandle_ResetTimeDilation, ResetTimerDel, ResetTimeDelay, false);
	}
	else
	{
		ResetTimeDilation(OtherActor);
	}
}

void AFPlayer::StartHitStop(AActor* OtherActor)
{
	if (bIsHitStopping)
	{
		return;
	}

	bIsHitStopping = true;
	CustomTimeDilation = 0.0f;
	OtherActor->CustomTimeDilation = 0.0f;

	if (AttackCamShake)
	{
		CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		CameraManager->PlayCameraShake(AttackCamShake);
	}

	if (ToSlowMoDelay > 0.0f)
	{
		FTimerDelegate SlowTimerDel;
		SlowTimerDel.BindUObject(this, &AFPlayer::SlowTimeDilation, OtherActor);

		GetWorldTimerManager().SetTimer(TimeHandle_SlowTimeDilation, SlowTimerDel, ToSlowMoDelay, false);
	}
	else
	{
		SlowTimeDilation(OtherActor);
	}
}

void AFPlayer::ChargeAttackTimerUpdate(float DeltaTime)
{
	if (bIsCharging)
	{
		ChargeTimer += 1.0f * DeltaTime;
	}
}
