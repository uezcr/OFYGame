// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/OFYGameMode.h"

#include "OFYExperienceManagerComponent.h"
#include "OFYGameState.h"
#include "OFYWorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/OFYExperienceDefinition.h"
#include "System/OFYAssetManager.h"

AOFYGameMode::AOFYGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GameStateClass = AOFYGameState::StaticClass();
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
