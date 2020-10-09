// Fill out your copyright notice in the Description page of Project Settings.


#include "FEDragon.h"
#include "FHealthComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectFinal/ProjectFinal.h"

AFEDragon::AFEDragon()
{
	CombatSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphereComp"));
	CombatSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphereComp->SetCollisionObjectType(COLLISION_ENEMY_ATTACK);
	CombatSphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatSphereComp->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Overlap);
	CombatSphereComp->SetupAttachment(GetMesh());
}

void AFEDragon::BeginPlay()
{
	Super::BeginPlay();
}

void AFEDragon::OnHealthChanged(UFHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Super::OnHealthChanged(OwnerHealthComp, Health, HealthDelta, DamageType, InstigatedBy, DamageCauser);
}
