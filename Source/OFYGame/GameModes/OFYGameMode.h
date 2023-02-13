// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "OFYGameMode.generated.h"

/**
 * 
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class OFYGAME_API AOFYGameMode : public AModularGameModeBase
{
	GENERATED_BODY()
public:
	AOFYGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	//~End of AGameModeBase interface


protected:

	//根据获取到的ExperienceDefinition的PrimaryAssetID加载ExperienceDefinition
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);
	
	//获取ExperienceDefinition的PrimaryAssetID
	void HandleMatchAssignmentIfNotExpectingOne();
	
};
