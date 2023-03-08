// Fill out your copyright notice in the Description page of Project Settings.


#include "System/OFYGameData.h"
#include "OFYAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYGameData)

UOFYGameData::UOFYGameData()
{
}

const UOFYGameData& UOFYGameData::Get()
{
	return UOFYAssetManager::Get().GetGameData();
}
