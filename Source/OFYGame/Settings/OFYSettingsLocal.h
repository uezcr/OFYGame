// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonInputBaseTypes.h"
#include "GameFramework/GameUserSettings.h"
#include "Input/OFYMappableConfigPair.h"
#include "OFYSettingsLocal.generated.h"

class UPlayerMappableInputConfig;
class UOFYLocalPlayer;

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

	// 设置要使用的控制器表示法，一个平台可能支持多种类型的控制器。
	// 例如，Win64游戏可以用XBox或Playstation控制器玩。
	UFUNCTION()
	void SetControllerPlatform(const FName InControllerPlatform);
	UFUNCTION()
    FName GetControllerPlatform() const;

	DECLARE_EVENT_OneParam(UOFYSettingsLocal, FInputConfigDelegate, const FLoadedMappableConfigPair& /*Config*/);

	/** 当一个新的输入配置被注册时被调用的委托 */
	FInputConfigDelegate OnInputConfigRegistered;

	/** 当一个注册的输入配置被激活时被调用的委托 */
	FInputConfigDelegate OnInputConfigActivated;
	
	/** 当一个注册的输入配置被停用时被调用的委托 */
	FInputConfigDelegate OnInputConfigDeactivated;
	
	/** 用设置注册给定的输入配置，使其对玩家可用。*/
	void RegisterInputConfig(ECommonInputType Type, const UPlayerMappableInputConfig* NewConfig, const bool bIsActive);
	
	/** 取消对给定的输入配置的注册，返回被移除的配置的数量。*/
	int32 UnregisterInputConfig(const UPlayerMappableInputConfig* ConfigToRemove);

	/** 获取一个具有特定名称的输入配置。如果该配置不存在，那么将返回nullptr。*/
	UFUNCTION(BlueprintCallable)
	const UPlayerMappableInputConfig* GetInputConfigByName(FName ConfigName) const;

	/** 获取所有当前注册的输入配置 */
	const TArray<FLoadedMappableConfigPair>& GetAllRegisteredInputConfigs() const { return RegisteredInputConfigs; }

	/**
	 * 获取所有符合输入类型的注册输入配置。
	 * 
	 * @param Type 要获取的配置类型，ECmonInputType::Count将包括所有配置。
	 * @param OutArray 数组，将用当前注册的符合该类型的输入配置来填充。
	 */
	void GetRegisteredInputConfigsOfType(ECommonInputType Type, OUT TArray<FLoadedMappableConfigPair>& OutArray) const;

	/**
	 * 返回任何与该键绑定的行动的显示名称
	 * 
	 * @param InKey 要检查当前键的映射的键。
	 * @param OutActionNames 数组，用于存储绑定键的动作的显示名称。
	 */
	void GetAllMappingNamesFromKey(const FKey InKey, TArray<FName>& OutActionNames);

	/**
	 * 将给定的键盘设置映射到新的键上
	 * 
	 * @param MappingName 你想改变的FPlayerMappableKeyOptions的名称。
	 * @param NewKey 绑定此选项的新键
	 * @param LocalPlayer 要重置键盘绑定的本地玩家
	 */
	void AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey, UOFYLocalPlayer* LocalPlayer);

	/**
	 * 将按键绑定重置为其输入映射环境中的默认值 
	 * 
	 * @param MappingName 你想改变的FPlayerMappableKeyOptions的名称。
	 * @param LocalPlayer 要重置键盘绑定的本地玩家
	 */
	void ResetKeybindingToDefault(const FName MappingName, UOFYLocalPlayer* LocalPlayer);

	/** 将所有按键绑定重置为其输入映射环境中的默认值。
	 * @param LocalPlayer 要重置键盘绑定的本地玩家
	 */
	void ResetKeybindingsToDefault(UOFYLocalPlayer* LocalPlayer);
	
	const TMap<FName, FKey>& GetCustomPlayerInputConfig() const { return CustomKeyboardConfig; }

private:

	/**
	 * 玩家正在使用的控制器的名称。 这是一个UCommonInputBaseControllerData的名字。
	 * 的名称，该名称在当前这个平台上是可用的。 游戏手柄的数据是按平台注册的，你会发现它们
	 *在<Platform>Game.ini文件中列在+ControllerData=...
	 */
	UPROPERTY(Config)
	FName ControllerPlatform;

	UPROPERTY(Config)
	FName ControllerPreset = TEXT("Default");

	/** 用户选择的当前输入配置的名称。 */
	UPROPERTY(Config)
	FName InputConfigName = TEXT("Default");

	/**
	 * 当前注册的输入配置的数组。这是由游戏功能插件填充的
	 * 
	 * @see UGameFeatureAction_AddInputConfig
	 */
	UPROPERTY(VisibleAnywhere)
	TArray<FLoadedMappableConfigPair> RegisteredInputConfigs;

	/** 被玩家设置的自定义按键映射的数组。默认情况下是空的。*/
	UPROPERTY(Config)
	TMap<FName, FKey> CustomKeyboardConfig;
};
