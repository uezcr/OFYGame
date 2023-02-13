// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "LoadingProcessInterface.h"

#include "OFYExperienceManagerComponent.generated.h"

class UOFYExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOFYExperienceLoaded, const UOFYExperienceDefinition* /*Experience*/);

enum class EOFYExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

/**
 * 
 */
UCLASS()
class UOFYExperienceManagerComponent final : public UGameStateComponent,public ILoadingProcessInterface
{
	GENERATED_BODY()
public:
	UOFYExperienceManagerComponent(const FObjectInitializer& ObjectInitializer=FObjectInitializer::Get());
	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	//~ILoadingProcessInterface interface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~End of ILoadingProcessInterface

	#if WITH_SERVER_CODE	
	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
#endif

	// 确保一旦ExperienceDefinition被加载，就会调用该委托。
	// 在其他的被调用之前，最先调用该委托
	// 然而，如果ExperienceDefinition已经被加载，则立即调用该委托
	void CallOrRegister_OnExperienceLoaded_HighPriority(FOnOFYExperienceLoaded::FDelegate&& Delegate);

	// 确保一旦ExperienceDefinition被加载，就会调用该委托。
	// 如果ExperienceDefinition已经被加载，则立即调用该委托
	void CallOrRegister_OnExperienceLoaded(FOnOFYExperienceLoaded::FDelegate&& Delegate);

	// 确保一旦ExperienceDefinition被加载，就会调用该委托。
	// 如果ExperienceDefinition已经被加载，则立即调用该委托
	void CallOrRegister_OnExperienceLoaded_LowPriority(FOnOFYExperienceLoaded::FDelegate&& Delegate);

	// 返回当前的ExperienceDefinition如果它已经完全加载了，否则断言
	// （即，如果你过早地调用它）。
	const UOFYExperienceDefinition* GetCurrentExperienceChecked() const;

	// 如果ExperienceDefinition被完全加载，返回true
	bool IsExperienceLoaded() const;

private:
	//该函数很重要，在GameMode初始化或其他地方修改ExperienceDefinition调用该函数，利用的是RPC函数的性质
	UFUNCTION()
	void OnRep_CurrentExperience();

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();

	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();

private:
	//当前ExperienceDefinition
	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperience)
	TObjectPtr<const UOFYExperienceDefinition> CurrentExperience;

	//加载状态
	EOFYExperienceLoadState LoadState = EOFYExperienceLoadState::Unloaded;

	//GameFeaturePlugins加载数量
	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;

	int32 NumObservedPausers = 0;
	int32 NumExpectedPausers = 0;

	/**
	 * Delegate called when the experience has finished loading just before others
	 * (e.g., subsystems that set up for regular gameplay)
	 */
	FOnOFYExperienceLoaded OnExperienceLoaded_HighPriority;

	/** Delegate called when the experience has finished loading */
	FOnOFYExperienceLoaded OnExperienceLoaded;

	/** Delegate called when the experience has finished loading */
	FOnOFYExperienceLoaded OnExperienceLoaded_LowPriority;
	
};
