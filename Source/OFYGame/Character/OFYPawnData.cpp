// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OFYPawnData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYPawnData)


UOFYPawnData::UOFYPawnData(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PawnClass = nullptr;
	InputConfig = nullptr;
	DefaultCameraMode = nullptr;
}
