// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FEnemy.h"
#include "FEDragon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFINAL_API AFEDragon : public AFEnemy
{
	GENERATED_BODY()
	
public:

	AFEDragon();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void OnHealthChanged(UFHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;
};
