// Fill out your copyright notice in the Description page of Project Settings.


#include "FEnemy.h"
#include "FHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ProjectFinal/ProjectFinal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

// Sets default values
AFEnemy::AFEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UFHealthComponent>(TEXT("HealthComp"));

	// Collision Setup
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->SetGenerateOverlapEvents(true);
	MeshComp->SetCollisionObjectType(COLLISION_ENEMY);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(COLLISION_WEAPON, ECollisionResponse::ECR_Overlap);

	bIsDied = false;
	DestroyLifeSpan = 10.0f;

	// Disable rotation yaw for AI Controller
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// Dissolve Init
	bStartDissolve = false;
	DissolveValue = 1.5f;
	DissolveTargetValue = -1.0f;
	DissolveInterpSpeed = 0.5f;
	DissolveDelayTime = 2.0f;
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
}

// Called to bind functionality to input
void AFEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFEnemy::Attack()
{
}

void AFEnemy::OnHealthChanged(UFHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bIsDied)
	{
		bIsDied = true;

		GetCharacterMovement()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
		SetLifeSpan(DestroyLifeSpan);
	}
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
