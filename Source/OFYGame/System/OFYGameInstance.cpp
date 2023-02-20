// Fill out your copyright notice in the Description page of Project Settings.


#include "System/OFYGameInstance.h"

#include "CommonSessionSubsystem.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Engine/LocalPlayer.h"
#include "GameplayTagContainer.h"
//#include "LyraGameplayTags.h"
#include "Misc/AssertionMacros.h"
//#include "Player/LyraPlayerController.h"
#include "Templates/Casts.h"
#include "Engine/LocalPlayer.h"

UOFYGameInstance::UOFYGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

bool UOFYGameInstance::CanJoinRequestedSession() const
{
	// Temporary first pass:  Always return true
	// This will be fleshed out to check the player's state
	if (!Super::CanJoinRequestedSession())
	{
		return false;
	}
	return true;
}

void UOFYGameInstance::Init()
{
	Super::Init();
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	// if (ensure(ComponentManager))
	// {
	// 	const FLyraGameplayTags& GameplayTags = FLyraGameplayTags::Get();
	//
	// 	ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
	// 	ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
	// 	ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
	// 	ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	// }
}

void UOFYGameInstance::Shutdown()
{
	Super::Shutdown();
}
