// Fill out your copyright notice in the Description page of Project Settings.


#include "FEnemy.h"
#include "FHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ProjectFinal/ProjectFinal.h"

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
}

// Called when the game starts or when spawned
void AFEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComp->OnHealthChanged.AddDynamic(this, &AFEnemy::OnHealthChanged);
}

// Called every frame
void AFEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	//UE_LOG(LogTemp, Log, TEXT("AFEnemy Changed"));
}

