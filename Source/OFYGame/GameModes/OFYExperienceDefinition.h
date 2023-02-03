// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OFYExperienceDefinition.generated.h"

class UGameFeatureAction;

/**
 * 
 */
UCLASS(BlueprintType, Const)
class OFYGAME_API UOFYExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UOFYExperienceDefinition();

public:
	UPROPERTY(EditDefaultsOnly,Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
	
	
};