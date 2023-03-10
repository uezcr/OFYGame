// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/OFYInputComponent.h"

#include "Containers/Map.h"
#include "EnhancedInputSubsystems.h"
#include "Input/OFYMappableConfigPair.h"
#include "InputCoreTypes.h"
#include "Player/OFYLocalPlayer.h"
#include "Settings/OFYSettingsLocal.h"
#include "Player/OFYLocalPlayer.h"
#include "Settings/OFYSettingsLocal.h"
#include "UObject/NameTypes.h"
#include "UObject/UnrealNames.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYInputComponent)

UOFYInputComponent::UOFYInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UOFYInputComponent::AddInputMappings(const UOFYInputConfig* InputConfig,
                                          UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);
	UOFYLocalPlayer* LocalPlayer = InputSubsystem->GetLocalPlayer<UOFYLocalPlayer>();
	check(LocalPlayer);

	if(UOFYSettingsLocal* LocalSettings = UOFYSettingsLocal::Get())
	{
		for(const TPair<FName,FKey>& Pair : LocalSettings->GetCustomPlayerInputConfig())
		{
			if(Pair.Key!=NAME_None && Pair.Value.IsValid())
			{
				InputSubsystem->AddPlayerMappedKey(Pair.Key,Pair.Value);
			}
		}
	}
}

void UOFYInputComponent::RemoveInputMappings(const UOFYInputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);
	UOFYLocalPlayer* LocalPlayer = InputSubsystem->GetLocalPlayer<UOFYLocalPlayer>();
	check(LocalPlayer);

	if(UOFYSettingsLocal* LocalSettings = UOFYSettingsLocal::Get())
	{
		const TArray<FLoadedMappableConfigPair>& Configs = LocalSettings->GetAllRegisteredInputConfigs();
		for(const FLoadedMappableConfigPair& Pair : Configs)
		{
			InputSubsystem->RemovePlayerMappableConfig(Pair.Config);
		}
		for(const TPair<FName,FKey>& Pair : LocalSettings->GetCustomPlayerInputConfig())
		{
			InputSubsystem->RemovePlayerMappedKey(Pair.Key);
		}
	}
}

void UOFYInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
