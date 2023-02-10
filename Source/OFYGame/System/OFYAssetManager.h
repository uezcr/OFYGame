// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "OFYAssetManager.generated.h"


struct FOFYBundles
{
	static const FName Equipped;
};
/**
 * 
 */
UCLASS(Config = Game)
class UOFYAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UOFYAssetManager();

	static UOFYAssetManager& Get();
	
};
