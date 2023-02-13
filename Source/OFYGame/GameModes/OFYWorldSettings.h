// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "OFYWorldSettings.generated.h"

class UOFYExperienceDefinition;

/**
 * 添加了ExperienceDefinition信息的世界设置
 */
UCLASS()
class OFYGAME_API AOFYWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
	AOFYWorldSettings(const FObjectInitializer& ObjectInitializer);
public:

	//返回默认Experience
	FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:

	//当服务器打开该地图时，如果没有被user-facing所覆盖，则使用的默认Experience
	UPROPERTY(EditAnywhere,Category=GameMode)
	TSoftClassPtr<UOFYExperienceDefinition> DefaultGameplayExperience;

public:
	
};
