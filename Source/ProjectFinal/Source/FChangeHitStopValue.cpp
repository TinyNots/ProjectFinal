// Fill out your copyright notice in the Description page of Project Settings.


#include "FChangeHitStopValue.h"
#include "FPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

UFChangeHitStopValue::UFChangeHitStopValue()
{
	HitStopPreset = EHitStop::Light;
	ToSlowMoDelay = 0.0f;
	SlowMoTimeDilation = 0.0f;
	ResetTimeDelay = 0.0f;
}

void UFChangeHitStopValue::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AFPlayer* MyPlayer = Cast<AFPlayer>(UGameplayStatics::GetPlayerPawn(MeshComp->GetWorld(), 0));
	if (MyPlayer && MyPlayer->bIsInGame)
	{
		MyPlayer->ChangeHitStop(HitStopPreset, ToSlowMoDelay, SlowMoTimeDilation, ResetTimeDelay);
	}
}
