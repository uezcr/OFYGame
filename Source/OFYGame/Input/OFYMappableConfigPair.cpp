// Fill out your copyright notice in the Description page of Project Settings.


#include "OFYMappableConfigPair.h"

#include "CommonUISettings.h"
#include "ICommonUIModule.h"
#include "Settings/OFYSettingsLocal.h"
#include "System/OFYAssetManager.h"
#include "PlayerMappableInputConfig.h"
#include "Templates/Casts.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYMappableConfigPair)

bool FMappableConfigPair::CanBeActivated() const
{
	const FGameplayTagContainer& PlatformTraits = ICommonUIModule::GetSettings().GetPlatformTraits();
	if(!DependentPlatformTraits.IsEmpty() && !PlatformTraits.HasAll(DependentPlatformTraits))
	{
		return false;
	}

	if(!ExcludedPlatformTraits.IsEmpty() && PlatformTraits.HasAny(ExcludedPlatformTraits))
	{
		return false;
	}
	return true;
}

bool FMappableConfigPair::RegisterPair(const FMappableConfigPair& Pair)
{
	UOFYAssetManager& AssetManager = UOFYAssetManager::Get();
	if(UOFYSettingsLocal* Settings = UOFYSettingsLocal::Get())
	{
		if(const UPlayerMappableInputConfig* LoadedConfig = AssetManager.GetAsset(Pair.Config))
		{
			Settings->RegisterInputConfig(Pair.Type,LoadedConfig,false);
			return true;
		}
	}

	return false;
}

void FMappableConfigPair::UnregisterPair(const FMappableConfigPair& Pair)
{
	UOFYAssetManager& AssetManager = UOFYAssetManager::Get();
	if(UOFYSettingsLocal* Settings = UOFYSettingsLocal::Get())
	{
		if(const UPlayerMappableInputConfig* LoadedConfig = AssetManager.GetAsset(Pair.Config))
		{
			Settings->UnregisterInputConfig(LoadedConfig);
		}
	}
}
