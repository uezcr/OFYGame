// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"

class UObject;

OFYGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogOFY, Log, All);
OFYGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogOFYExperience, Log, All);
OFYGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogOFYAbilitySystem, Log, All);
OFYGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogOFYTeams, Log, All);

OFYGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);