// Fill out your copyright notice in the Description page of Project Settings.


#include "FEDragon.h"
#include "FHealthComponent.h"

AFEDragon::AFEDragon()
{

}

void AFEDragon::BeginPlay()
{
	Super::BeginPlay();
}

void AFEDragon::OnHealthChanged(UFHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Super::OnHealthChanged(OwnerHealthComp, Health, HealthDelta, DamageType, InstigatedBy, DamageCauser);
}
