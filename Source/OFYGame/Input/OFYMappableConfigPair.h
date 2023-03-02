// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonInputBaseTypes.h"
#include "GameplayTagContainer.h"
#include "UObject/SoftObjectPtr.h"

#include "OFYMappableConfigPair.generated.h"


class UPlayerMappableInputConfig;


/** 一个容器，用于组织加载的"玩家可映射配置"到他们的CommonUI输入类型 */
USTRUCT(BlueprintType)
struct FLoadedMappableConfigPair
{
	GENERATED_BODY()
	FLoadedMappableConfigPair() = default;
	FLoadedMappableConfigPair(const UPlayerMappableInputConfig* InConfig,ECommonInputType InType,const bool InIsActive)
		:Config(InConfig)
	    ,Type(InType)
	    ,bIsActive(InIsActive)
	{}
	/** 玩家可映射输入配置，应该应用于增强型输入子系统 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<const UPlayerMappableInputConfig> Config = nullptr;

	/** 此映射配置应适用于的设备类型 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECommonInputType Type = ECommonInputType::Count;

	/** 如果这个配置当前是活动的。当一个配置所拥有的GFA处于活动状态时，它就被标记为活动状态 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsActive = false;
};

/** 一个容器，用于组织潜在的未加载的"玩家可映射配置"到他们的CommonUI输入类型 */
USTRUCT(BlueprintType)
struct FMappableConfigPair
{
	GENERATED_BODY()

	FMappableConfigPair() = default;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	/**
	 * 这是配置的类型，有助于通过当前输入设备过滤出配置，比如设置屏幕。
	 * 或者如果你只想在某个特定的输入类型被使用时应用这个配置。
	 */
	UPROPERTY(EditAnywhere)
	ECommonInputType Type = ECommonInputType::Count;

	/**
	 * 为了激活这个输入，必须设置的平台特征的容器。
	 * 如果该平台没有指定的特征之一，它仍然可以被注册，但不能被激活。
	 */
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DependentPlatformTraits;

	/**
	 * 如果当前平台有这些特征，那么这个配置将不会被激活。
	 */
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer ExcludedPlatformTraits;

	/** 如果为真，那么这个输入配置将在它的相关游戏功能被激活时被激活。
	  * 这通常是理想的行为。
	  */
	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;

	/** 如果这个配置对可以根据当前的平台特征和设置被激活，则返回真。*/
	bool CanBeActivated() const;
	
	/**
	 *将给定的配置映射与本地设置进行注册
	 */
	static bool RegisterPair(const FMappableConfigPair& Pair);

	/**
	 * 取消给定的配置映射与本地设置的注册
	 */
	static void UnregisterPair(const FMappableConfigPair& Pair);
};
