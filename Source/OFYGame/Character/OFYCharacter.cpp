// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OFYCharacter.h"

#include "OFYPawnExtensionComponent.h"
#include "Camera/OFYCameraComponent.h"
#include "GameFramework/PlayerState.h"
#include "Player/OFYPlayerController.h"

#define M_PI 3.14159265358979323846


#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYCharacter)

const double testx1 = 25.225859669;
const double testy1 = 103.401097565;

const double testx2 = 25.225859690;
const double testy2 = 103.401097567;

AOFYCharacter::AOFYCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	PawnExtComponent = CreateDefaultSubobject<UOFYPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	
	CameraComponent = CreateDefaultSubobject<UOFYCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
}

AOFYPlayerController* AOFYCharacter::GetOFYPlayerController() const
{
	return CastChecked<AOFYPlayerController>(Controller,ECastCheckedType::NullAllowed);
}

void AOFYCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PawnExtComponent->HandleControllerChanged();
}

void AOFYCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AOFYCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PawnExtComponent->SetupPlayerInputComponent();
}

