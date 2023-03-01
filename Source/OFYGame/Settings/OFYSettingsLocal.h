// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonInputBaseTypes.h"
#include "GameFramework/GameUserSettings.h"
#include "OFYSettingsLocal.generated.h"

USTRUCT()
struct FOFYScalabilitySnapshot
{
	GENERATED_BODY()

	FOFYScalabilitySnapshot();

	Scalability::FQualityLevels Qualities;
	bool bActive = false;
	bool bHasOverrides = false;
};

/**
 * 
 */
UCLASS()
class UOFYSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()
public:

	//构造
	UOFYSettingsLocal();

	static UOFYSettingsLocal* Get();

	//KeyBindings
	
public:


	const TMap<FName, FKey>& GetCustomPlayerInputConfig() const { return CustomKeyboardConfig; }

private:

	UPROPERTY(Config)
	TMap<FName, FKey> CustomKeyboardConfig;
};
