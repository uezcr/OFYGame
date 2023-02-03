// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "OFYWorldSettings.generated.h"

class UOFYExperienceDefinition;

/**
 * 
 */
UCLASS()
class OFYGAME_API AOFYWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
	AOFYWorldSettings(const FObjectInitializer& ObjectInitializer);
public:
	FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
	UPROPERTY(EditAnywhere,Category=GameMode)
	TSoftClassPtr<UOFYExperienceDefinition> DefaultGameplayExperience;

public:
	
};
