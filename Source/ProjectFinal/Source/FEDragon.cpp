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

	CombatSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFEDragon::CombatOnOverlapBegin);
}

void AFEDragon::OnHealthChanged(UFHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Super::OnHealthChanged(OwnerHealthComp, Health, HealthDelta, DamageType, InstigatedBy, DamageCauser);
}

void AFEDragon::EnableAttackCollision()
{
	CombatSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AFEDragon::DisableAttackCollision()
{
	CombatSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFEDragon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::CombatOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ACharacter* MyCharacter = Cast<ACharacter>(OtherActor);
	if (MyCharacter)
	{
		CombatSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
