// Fill out your copyright notice in the Description page of Project Settings.


#include "System/OFYAssetManager.h"



#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYAssetManager)


const FName FOFYBundles::Equipped("Equipped");

UOFYAssetManager::UOFYAssetManager()
{
}

UOFYAssetManager& UOFYAssetManager::Get()
{
	check(GEngine);
	if (UOFYAssetManager* Singleton = Cast<UOFYAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}
	return *NewObject<UOFYAssetManager>();
}
