// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "GFAction_WorldActionBase.h"
#include "HAL/Platform.h"
#include "UObject/ObjectKey.h"
#include "UObject/UObjectGlobals.h"
#include "GFAction_SplitscreenConfig.generated.h"


class UObject;
struct FGameFeatureDeactivatingContext;
struct FGameFeatureStateChangeContext;
struct FWorldContext;
/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Splitscreen Config"))
class UGFAction_SplitscreenConfig final :public UGFAction_WorldActionBase
{
	GENERATED_BODY()

public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~ End UGameFeatureAction interface

	//~ Begin UGameFeatureAction_WorldActionBase interface
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	//~ End UGameFeatureAction_WorldActionBase interface

	public:
	UPROPERTY(EditAnywhere, Category=Action)
	bool bDisableSplitscreen = true;

private:
	TArray<FObjectKey> LocalDisableVotes;

	static TMap<FObjectKey, int32> GlobalDisableVotes;
	
};
