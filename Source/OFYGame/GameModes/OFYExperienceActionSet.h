// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OFYExperienceActionSet.generated.h"

class UGameFeatureAction;

/**
 * 
 */
UCLASS(BlueprintType,NotBlueprintable)
class UOFYExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UOFYExperienceActionSet();

public:
	UPROPERTY(EditAnywhere,Instanced,Category="Action to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	UPROPERTY(EditAnywhere,Category="Feature Dependencies")
	TArray<FString> GameFeaturesToEnable;
	
};
