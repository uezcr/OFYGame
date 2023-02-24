// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OFYCharacter.h"

#include "Player/OFYPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYCharacter)

AOFYCharacter::AOFYCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

AOFYPlayerController* AOFYCharacter::GetOFYPlayerController() const
{
	return CastChecked<AOFYPlayerController>(Controller,ECastCheckedType::NullAllowed);
}

