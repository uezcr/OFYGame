// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/OFYGameState.h"

#include "OFYExperienceManagerComponent.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYGameState)
class APlayerState;
class FLifetimeProperty;

AOFYGameState::AOFYGameState(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	//创建组件
	ExperienceManagerComponent = CreateDefaultSubobject<UOFYExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
	
}
void AOFYGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AOFYGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AOFYGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AOFYGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
}

void AOFYGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
}


