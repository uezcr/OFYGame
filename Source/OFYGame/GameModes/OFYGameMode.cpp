// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/OFYGameMode.h"

#include "OFYExperienceManagerComponent.h"
#include "OFYGameState.h"
#include "OFYWorldSettings.h"
#include "Character/OFYCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/OFYExperienceDefinition.h"
#include "Player/OFYPlayerController.h"
#include "Player/OFYPlayerState.h"
#include "System/OFYAssetManager.h"
#include "UI/OFYHUD.h"

AOFYGameMode::AOFYGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GameStateClass = AOFYGameState::StaticClass();
	PlayerControllerClass = AOFYPlayerController::StaticClass();
	PlayerStateClass = AOFYPlayerState::StaticClass();
	DefaultPawnClass = AOFYCharacter::StaticClass();
	HUDClass = AOFYHUD::StaticClass();
}

const UOFYPawnData* AOFYGameMode::GetPawnDataForController(const AController* InController) const
{
	//看Pawn的数据是否已经在PlayerState上设置了
	if (InController != nullptr)
	{
		if (const AOFYPlayerState* LyraPS = InController->GetPlayerState<AOFYPlayerState>())
		{
			if (const UOFYPawnData* PawnData = LyraPS->GetPawnData<UOFYPawnData>())
			{
				return PawnData;
			}
		}
	}

	// 如果没有，就回到当前Experience的默认值。
	check(GameState);
	UOFYExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UOFYExperienceManagerComponent>();
	check(ExperienceComponent);

	if (ExperienceComponent->IsExperienceLoaded())
	{
		const UOFYExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}

		// Experience已经加载，但仍然没有Pawn的数据，暂时退回到默认状态。
		return UOFYAssetManager::Get().GetDefaultPawnData();
	}

	// Experience经验还没有加载，所以没有Pawn的数据可言
	return nullptr;
}

void AOFYGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this,&ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}



void AOFYGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;
	UWorld* World = GetWorld();

	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UOFYExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
		ExperienceIdSource = TEXT("OptionsString");
	}

	//TODO DeveloperSettings
	
	if (!ExperienceId.IsValid())
	{
		FString ExperienceFromCommandLine;
		if (FParse::Value(FCommandLine::Get(), TEXT("Experience="), ExperienceFromCommandLine))
		{
			ExperienceId = FPrimaryAssetId::ParseTypeAndName(ExperienceFromCommandLine);
			ExperienceIdSource = TEXT("CommandLine");
		}
	}

	if (!ExperienceId.IsValid())
	{
		if (AOFYWorldSettings* TypedWorldSettings = Cast<AOFYWorldSettings>(GetWorldSettings()))
		{
			ExperienceId = TypedWorldSettings->GetDefaultGameplayExperience();
			ExperienceIdSource = TEXT("WorldSettings");
		}
	}

	UOFYAssetManager& AssetManager = UOFYAssetManager::Get();
	FAssetData Dummy;
	if (ExperienceId.IsValid() && !AssetManager.GetPrimaryAssetData(ExperienceId, /*out*/ Dummy))
	{
		UE_LOG(LogTemp, Error, TEXT("EXPERIENCE: Wanted to use %s but couldn't find it, falling back to the default)"), *ExperienceId.ToString());
		ExperienceId = FPrimaryAssetId();
	}

	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("OFYExperienceDefinition"), FName("B_OFYDefaultExperience"));
		ExperienceIdSource = TEXT("Default");
	}
	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}

void AOFYGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
#if WITH_SERVER_CODE
	if (ExperienceId.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Identified experience %s (Source: %s)"), *ExperienceId.ToString(), *ExperienceIdSource);

		UOFYExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UOFYExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->ServerSetCurrentExperience(ExperienceId);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
#endif
}
