// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "OFYExperienceManager.generated.h"



/**
 * 
 */
UCLASS(MinimalAPI)
class UOFYExperienceManager : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	static void NotifyOfPluginActivation(const FString PluginURL) {}
	static bool RequestToDeactivatePlugin(const FString PluginURL) { return true; }

private:
	TMap<FString,int32> GameFeaturePluginRequestCountMap;
	
};
