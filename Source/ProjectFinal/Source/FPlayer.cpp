// Fill out your copyright notice in the Description page of Project Settings.


#include "FPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "FHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
AFPlayer::AFPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponSocketName = "hand_rSocket";

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(GetRootComponent());

	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 20.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<UFHealthComponent>(TEXT("HealthComp"));

	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComp"));
	WeaponMeshComp->SetupAttachment(GetMesh(), WeaponSocketName);

	bUseControllerRotationYaw = false;
	UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement();
	if (CharacterMovementComp)
	{
		CharacterMovementComp->bOrientRotationToMovement = true;
		CharacterMovementComp->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
		CharacterMovementComp->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}

	CurrentCombo = 0;
	bIsNextAttackPressed = false;

	// Animation Init
	HeadInterpSpeed = 3.0f;
	HeadDegreeThreshold = 120.0f;
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

	AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("There's problem with your AnimInstace"));
	}
}

void AFPlayer::HandleCombo()
{
	if (bIsNextAttackPressed)
	{
		ToNextAttack();
		bIsNextAttackPressed = false;
	}
	else
	{
		CurrentCombo = 0;
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

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &AFPlayer::Attack);
}

UCameraComponent* AFPlayer::GetCameraComponent()
{
	return CameraComp;
}

void AFPlayer::MoveForward(float Value)
{
	AddMovementInput(CameraComp->GetForwardVector() * Value);
}

void AFPlayer::MoveRight(float Value)
{
	AddMovementInput(CameraComp->GetRightVector() * Value);
}

void AFPlayer::Attack()
{
	if (AnimInstance && AttackMontage)
	{
		CurrentCombo++;

		FString StringComboName = "Combo0";
		StringComboName.AppendInt(CurrentCombo);
		FName ComboSectionName = FName(*StringComboName);

		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(ComboSectionName, AttackMontage);
	}
}

void AFPlayer::ToNextAttack()
{
	if (AnimInstance && AttackMontage)
	{
		CurrentCombo++;

		FString StringComboName = "Combo0";
		StringComboName.AppendInt(CurrentCombo);
		FName ComboSectionName = FName(*StringComboName);

		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(ComboSectionName, AttackMontage);
	}
}
