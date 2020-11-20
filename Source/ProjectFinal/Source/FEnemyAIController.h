// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FEnemyAIController.generated.h"

class AFEnemy;

UCLASS()
class PROJECTFINAL_API AFEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AFEnemyAIController();

protected:

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

private:

	AFEnemy* Owner;

	void UpdateProperties();

};
