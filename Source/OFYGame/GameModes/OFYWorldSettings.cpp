// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/OFYWorldSettings.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Misc/UObjectToken.h"
#include "Logging/TokenizedMessage.h"
#include "Logging/MessageLog.h"
#include "OFYExperienceDefinition.h"
#include "Engine/AssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYWorldSettings)

AOFYWorldSettings::AOFYWorldSettings(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

FPrimaryAssetId AOFYWorldSettings::GetDefaultGameplayExperience() const
{
	FPrimaryAssetId Result;
	if (!DefaultGameplayExperience.IsNull())
	{
		Result = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayExperience.ToSoftObjectPath());

		if (!Result.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("%s.DefaultGameplayExperience is %s but that failed to resolve into an asset ID (you might need to add a path to the Asset Rules in your game feature plugin or project settings"),
				*GetPathNameSafe(this), *DefaultGameplayExperience.ToString());
		}
	}
	return Result;
}
