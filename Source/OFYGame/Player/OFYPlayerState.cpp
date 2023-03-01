// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OFYPlayerState.h"

#include "Net/UnrealNetwork.h"

AOFYPlayerState::AOFYPlayerState(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void AOFYPlayerState::OnRep_PawnData()
{
}

void AOFYPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
}