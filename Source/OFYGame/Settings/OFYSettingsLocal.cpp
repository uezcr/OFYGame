// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/OFYSettingsLocal.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "NativeGameplayTags.h"
#include "Player/OFYLocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYSettingsLocal)

//////////////////////////////////////////////////////////////////////

// #if WITH_EDITOR
// static TAutoConsoleVariable<bool> CVarApplyFrameRateSettingsInPIE(TEXT("OFY.Settings.ApplyFrameRateSettingsInPIE"),
// 	false,
// 	TEXT("Should we apply frame rate settings in PIE?"),
// 	ECVF_Default);
//
// static TAutoConsoleVariable<bool> CVarApplyFrontEndPerformanceOptionsInPIE(TEXT("OFY.Settings.ApplyFrontEndPerformanceOptionsInPIE"),
// 	false,
// 	TEXT("Do we apply front-end specific performance options in PIE?"),
// 	ECVF_Default);
//
// static TAutoConsoleVariable<bool> CVarApplyDeviceProfilesInPIE(TEXT("OFY.Settings.ApplyDeviceProfilesInPIE"),
// 	false,
// 	TEXT("Should we apply experience/platform emulated device profiles in PIE?"),
// 	ECVF_Default);
// #endif
//
// //////////////////////////////////////////////////////////////////////
// // Console frame pacing
//
// static TAutoConsoleVariable<int32> CVarDeviceProfileDrivenTargetFps(
// 	TEXT("OFY.DeviceProfile.Console.TargetFPS"),
// 	-1,
// 	TEXT("Target FPS when being driven by device profile"),
// 	ECVF_Default | ECVF_Preview);
//
// static TAutoConsoleVariable<int32> CVarDeviceProfileDrivenFrameSyncType(
// 	TEXT("OFY.DeviceProfile.Console.FrameSyncType"),
// 	-1,
// 	TEXT("Sync type when being driven by device profile. Corresponds to r.GTSyncType"),
// 	ECVF_Default | ECVF_Preview);
//
// //////////////////////////////////////////////////////////////////////
// // Mobile frame pacing
//
// static TAutoConsoleVariable<int32> CVarDeviceProfileDrivenMobileDefaultFrameRate(
// 	TEXT("OFY.DeviceProfile.Mobile.DefaultFrameRate"),
// 	30,
// 	TEXT("Default FPS when being driven by device profile"),
// 	ECVF_Default | ECVF_Preview);
//
// static TAutoConsoleVariable<int32> CVarDeviceProfileDrivenMobileMaxFrameRate(
// 	TEXT("OFY.DeviceProfile.Mobile.MaxFrameRate"),
// 	30,
// 	TEXT("Max FPS when being driven by device profile"),
// 	ECVF_Default | ECVF_Preview);
//
// //////////////////////////////////////////////////////////////////////
//
// static TAutoConsoleVariable<FString> CVarMobileQualityLimits(
// 	TEXT("OFY.DeviceProfile.Mobile.OverallQualityLimits"),
// 	TEXT(""),
// 	TEXT("List of limits on resolution quality of the form \"FPS:MaxQuality,FPS2:MaxQuality2,...\", kicking in when FPS is at or above the threshold"),
// 	ECVF_Default | ECVF_Preview);
//
// static TAutoConsoleVariable<FString> CVarMobileResolutionQualityLimits(
// 	TEXT("OFY.DeviceProfile.Mobile.ResolutionQualityLimits"),
// 	TEXT(""),
// 	TEXT("List of limits on resolution quality of the form \"FPS:MaxResQuality,FPS2:MaxResQuality2,...\", kicking in when FPS is at or above the threshold"),
// 	ECVF_Default | ECVF_Preview);
//
// static TAutoConsoleVariable<FString> CVarMobileResolutionQualityRecommendation(
// 	TEXT("OFY.DeviceProfile.Mobile.ResolutionQualityRecommendation"),
// 	TEXT("0:75"),
// 	TEXT("List of limits on resolution quality of the form \"FPS:Recommendation,FPS2:Recommendation2,...\", kicking in when FPS is at or above the threshold"),
// 	ECVF_Default | ECVF_Preview);
//
// //////////////////////////////////////////////////////////////////////

UOFYSettingsLocal::UOFYSettingsLocal()
{
}

UOFYSettingsLocal* UOFYSettingsLocal::Get()
{
	return GEngine ? CastChecked<UOFYSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
}

void UOFYSettingsLocal::SetControllerPlatform(const FName InControllerPlatform)
{
	if(ControllerPlatform!=InControllerPlatform)
	{
		ControllerPlatform = InControllerPlatform;
		if(UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(GetTypedOuter<ULocalPlayer>()))
		{
			InputSubsystem->SetGamepadInputType(ControllerPlatform);
		}
	}
}

FName UOFYSettingsLocal::GetControllerPlatform() const
{
	return ControllerPlatform;
}

void UOFYSettingsLocal::RegisterInputConfig(ECommonInputType Type, const UPlayerMappableInputConfig* NewConfig,
                                            const bool bIsActive)
{
	if(NewConfig)
	{
		const int32 ExistingConfigIdx = RegisteredInputConfigs.IndexOfByPredicate([&NewConfig](const FLoadedMappableConfigPair& Pair){return Pair.Config == NewConfig;});
		if(ExistingConfigIdx == INDEX_NONE)
		{
			const int32 NumAdded = RegisteredInputConfigs.Add(FLoadedMappableConfigPair(NewConfig, Type, bIsActive));
			if(NumAdded!=INDEX_NONE)
			{
				OnInputConfigRegistered.Broadcast(RegisteredInputConfigs[NumAdded]);
			}
		}

	}
}

int32 UOFYSettingsLocal::UnregisterInputConfig(const UPlayerMappableInputConfig* ConfigToRemove)
{
	if(ConfigToRemove)
	{
		const int32 Index = RegisteredInputConfigs.IndexOfByPredicate([&ConfigToRemove](const FLoadedMappableConfigPair& Pair) { return Pair.Config == ConfigToRemove; } );
		if(Index != INDEX_NONE)
		{
			RegisteredInputConfigs.RemoveAt(Index);
			return 1;
		}
	}
	return INDEX_NONE;
}

const UPlayerMappableInputConfig* UOFYSettingsLocal::GetInputConfigByName(FName ConfigName) const
{
	for(const FLoadedMappableConfigPair& Pair : RegisteredInputConfigs)
	{
		if(Pair.Config->GetConfigName()==ConfigName)
		{
			return Pair.Config;
		}
	}
	return nullptr;
}

void UOFYSettingsLocal::GetRegisteredInputConfigsOfType(ECommonInputType Type,
	TArray<FLoadedMappableConfigPair>& OutArray) const
{
	OutArray.Empty();

	if(Type == ECommonInputType::Count)
	{
		OutArray = RegisteredInputConfigs;
		return;
	}
	for(const FLoadedMappableConfigPair& Pair:RegisteredInputConfigs)
	{
		if(Pair.Type == Type)
		{
			OutArray.Emplace(Pair);
		}
	}
}

void UOFYSettingsLocal::GetAllMappingNamesFromKey(const FKey InKey, TArray<FName>& OutActionNames)
{
	if(InKey == EKeys::Invalid)
	{
		return;
	}

	for(const FLoadedMappableConfigPair& Pair : RegisteredInputConfigs)
	{
		if(Pair.Type == ECommonInputType::MouseAndKeyboard)
		{
			for (const FEnhancedActionKeyMapping& Mapping : Pair.Config->GetPlayerMappableKeys())
			{
				FName MappingName(Mapping.PlayerMappableOptions.DisplayName.ToString());
				FName ActionName = Mapping.PlayerMappableOptions.Name;
				// make sure it isn't custom bound as well
				if (const FKey* MappingKey = CustomKeyboardConfig.Find(ActionName))
				{
					if (*MappingKey == InKey)
					{
						OutActionNames.Add(MappingName);
					}
				}
				else
				{
					if (Mapping.Key == InKey)
					{
						OutActionNames.Add(MappingName);
					}
				}
			}
		}
	}
}

void UOFYSettingsLocal::AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey,
	UOFYLocalPlayer* LocalPlayer)
{
	if (MappingName == NAME_None)
	{
		return;
	}
	
	if (InputConfigName != TEXT("Custom"))
	{
		// Copy Presets.
		if (const UPlayerMappableInputConfig* DefaultConfig = GetInputConfigByName(TEXT("Default")))
		{
			for (const FEnhancedActionKeyMapping& Mapping : DefaultConfig->GetPlayerMappableKeys())
			{
				// 确保映射有一个有效的名字，有可能是一个空名字。
				// 如果有人将一个映射标记为 "玩家可映射"，但删除了默认字段的值
				if (Mapping.PlayerMappableOptions.Name != NAME_None)
				{
					CustomKeyboardConfig.Add(Mapping.PlayerMappableOptions.Name, Mapping.Key);
				}
			}
		}
		
		InputConfigName = TEXT("Custom");
	} 

	if (FKey* ExistingMapping = CustomKeyboardConfig.Find(MappingName))
	{
		// 把按键换成新的
		CustomKeyboardConfig[MappingName] = NewKey;
	}
	else
	{
		CustomKeyboardConfig.Add(MappingName, NewKey);
	}

	// 告诉这个本地玩家的增强型输入子系统，我们应该重新映射一些输入
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->AddPlayerMappedKey(MappingName, NewKey);
	}
}

void UOFYSettingsLocal::ResetKeybindingToDefault(const FName MappingName, UOFYLocalPlayer* LocalPlayer)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->RemovePlayerMappedKey(MappingName);
	}
}

void UOFYSettingsLocal::ResetKeybindingsToDefault(UOFYLocalPlayer* LocalPlayer)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->RemoveAllPlayerMappedKeys();
	}
}



