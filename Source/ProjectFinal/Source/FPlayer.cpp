// Fill out your copyright notice in the Description page of Project Settings.


#include "FPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "FHealthComponent.h"

// Sets default values
AFPlayer::AFPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(GetRootComponent());

	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 20.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<UFHealthComponent>(TEXT("HealthComp"));
}

// Called when the game starts or when spawned
void AFPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Set camera pitch rotation
	if (GetWorld())
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		if (CameraManager)
		{
			CameraManager->ViewPitchMin = -45.0f;
			CameraManager->ViewPitchMax = 45.0f;
		}
	}
}

// Called every frame
void AFPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPlayer::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AFPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AFPlayer::AddControllerYawInput);
}

void AFPlayer::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AFPlayer::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

