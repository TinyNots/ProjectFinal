// Fill out your copyright notice in the Description page of Project Settings.


#include "FEnemy.h"
#include "FHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ProjectFinal/ProjectFinal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPlayer.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFEnemy::AFEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UFHealthComponent>(TEXT("HealthComp"));

	// Collision Setup
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetGenerateOverlapEvents(false);
	CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->SetGenerateOverlapEvents(true);
	MeshComp->SetCollisionObjectType(COLLISION_ENEMY);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(COLLISION_WEAPON, ECollisionResponse::ECR_Overlap);

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComp"));
	HealthWidgetComp->SetupAttachment(GetCapsuleComponent());

	bIsDied = false;
	DestroyLifeSpan = 10.0f;

	// Disable rotation yaw for AI Controller
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Dissolve Init
	bStartDissolve = false;
	DissolveValue = 1.5f;
	DissolveTargetValue = -1.0f;
	DissolveInterpSpeed = 0.5f;
	DissolveDelayTime = 2.0f;

	AttackDamage = 10.0f;
	bCanKnockBack = true;
	KnockBackPoPitchAngle = 45.0f;
	KnockBackPower = 500.0f;
}

// Called when the game starts or when spawned
void AFEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComp->OnHealthChanged.AddDynamic(this, &AFEnemy::OnHealthChanged);

	AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("There's problem with your AnimInstace"));
	}

	if (BehaviorTree)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->RunBehaviorTree(BehaviorTree);
		}
	}

	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (!CameraManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to get camera manager"));
	}
}

void AFEnemy::StartDissolve()
{
	bStartDissolve = true;
}

// Called every frame
void AFEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DissolveInterpUpdate(DeltaTime);

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(HealthWidgetComp->GetComponentLocation(), CameraManager->GetCameraLocation());
	NewRotation.Roll = 0.0f;
	HealthWidgetComp->SetWorldRotation(NewRotation);
}

// Called to bind functionality to input
void AFEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFEnemy::Attack()
{
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection("Attack01");
	}
}

void AFEnemy::OnHealthChanged(UFHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	PlayHitReaction();

	if (Health <= 0.0f && !bIsDied)
	{
		if (AnimInstance && AttackMontage)
		{
			AnimInstance->StopAllMontages(0.0f);
		}

		bIsDied = true;

		GetCharacterMovement()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
		SetLifeSpan(DestroyLifeSpan);
	}
}

void AFEnemy::EnableAttackCollision()
{
	
}

void AFEnemy::DisableAttackCollision()
{

}

void AFEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && DamageTypeClass)
	{
		UGameplayStatics::ApplyDamage(OtherActor, AttackDamage, GetController(), this, DamageTypeClass);

		AFPlayer* MyPlayer = Cast<AFPlayer>(OtherActor);
		if (MyPlayer)
		{
			FRotator LaunchDirection = GetActorRotation();
			LaunchDirection.Pitch += KnockBackPoPitchAngle;
			FVector LaunchVelocity = LaunchDirection.Vector() * KnockBackPower;

			MyPlayer->LaunchCharacter(LaunchVelocity, true, true);

			MyPlayer->PlayHitReaction();
		}

		/*StartHitStop(OtherActor);*/

		/*if (AttackHitSound)
		{
			FVector SoundLocation = FVector(SweepResult.Location);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), (USoundBase*)AttackHitSound, SoundLocation);
		}*/
	}
}

const bool AFEnemy::GetIsGettingHit() const
{
	if (AnimInstance && GetHitMontage)
	{
		return AnimInstance->Montage_IsPlaying(GetHitMontage);
	}
	return false;
}

void AFEnemy::DissolveInterpUpdate(float DeltaTime)
{
	if (bStartDissolve)
	{
		DissolveValue = FMath::FInterpTo(DissolveValue, DissolveTargetValue, DeltaTime, DissolveInterpSpeed);
		GetMesh()->SetScalarParameterValueOnMaterials("Dissolve", DissolveValue);

		if (DissolveValue <= DissolveTargetValue)
		{
			bStartDissolve = false;
		}
	}
}

void AFEnemy::PlayHitReaction()
{
	if (AnimInstance && GetHitMontage)
	{
		DisableAttackCollision();
		AnimInstance->Montage_Play(GetHitMontage);
	}
}
