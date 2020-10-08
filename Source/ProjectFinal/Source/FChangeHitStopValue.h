// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnumHeader.h"
#include "FChangeHitStopValue.generated.h"

enum class EHitStop : uint8;

UCLASS()
class PROJECTFINAL_API UFChangeHitStopValue : public UAnimNotify
{
	GENERATED_BODY()

public:

	UFChangeHitStopValue();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	EHitStop HitStopPreset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	float ToSlowMoDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	float SlowMoTimeDilation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	float ResetTimeDelay;
};
