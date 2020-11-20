// Fill out your copyright notice in the Description page of Project Settings.


#include "FEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "FEnemy.h"

AFEnemyAIController::AFEnemyAIController()
{

}

void AFEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Owner = Cast<AFEnemy>(GetPawn());
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn cast to FEnemy is failed"));
	}
}

void AFEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateProperties();
}

void AFEnemyAIController::UpdateProperties()
{
	Blackboard->SetValueAsBool(FName("bIsGettingHit"), Owner->GetIsGettingHit());
}
