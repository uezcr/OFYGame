// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/OFYGameMode.h"

#include "OFYExperienceManagerComponent.h"
#include "OFYGameState.h"
#include "OFYWorldSettings.h"
#include "TimerManager.h"
#include "Character/OFYCharacter.h"
#include "Character/OFYPawnData.h"
#include "Character/OFYPawnExtensionComponent.h"
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
	//检查Pawn的数据是否已经在PlayerState上设置了
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

void AOFYGameMode::OnExperienceLoaded(const UOFYExperienceDefinition* CurrentExperience)
{
	// 生成任何已经加入的玩家
	//@TODO: 这里我们只处理*玩家的控制器，但在GetDefaultPawnClassForController_Implementation中我们跳过了所有控制器。
	// GetDefaultPawnClassForController_Implementation可能只为玩家调用。
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if((PC!=nullptr)&&(PC->GetPawn() == nullptr))
		{
			if(PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

bool AOFYGameMode::IsExperienceLoaded() const
{
	check(GameState);
	UOFYExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UOFYExperienceManagerComponent>();
	check(ExperienceComponent);
	return ExperienceComponent->IsExperienceLoaded();
}

UClass* AOFYGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if(const UOFYPawnData* PawnData = GetPawnDataForController(InController))
	{
		if(PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* AOFYGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient; // 不要把默认的玩家Pawn保存到地图中
	SpawnInfo.bDeferConstruction = true;

	if(UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if(APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass,SpawnTransform,SpawnInfo))
		{
			if(UOFYPawnExtensionComponent* PawnExtComp = UOFYPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if(const UOFYPawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}
			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}
	return nullptr;
}

bool AOFYGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void AOFYGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// 推迟启动新玩家，直到Experience加载完毕
	//（在此之前登录的玩家将由OnExperienceLoaded启动）
	if(IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

AActor* AOFYGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	//TODO ULyraPlayerSpawningManagerComponent
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AOFYGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	//TODO ULyraPlayerSpawningManagerComponent
	Super::FinishRestartPlayer(NewPlayer, StartRotation);
}

bool AOFYGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return ControllerCanRestart(Player);
}

bool AOFYGameMode::ControllerCanRestart(AController* Controller)
{
	if(APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if(!Super::PlayerCanRestart_Implementation(PC))
		{
			return false;
		}
	}
	else
	{
		if((Controller == nullptr) || Controller->IsPendingKillPending())
		{
			return false;
		}
	}

	//TODO ULyraPlayerSpawningManagerComponent
	return true;
}

void AOFYGameMode::InitGameState()
{
	Super::InitGameState();
	UOFYExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UOFYExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnOFYExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void AOFYGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	OnGameModeCombinedPostLoginDelegate.Broadcast(this,NewPlayer);
}

void AOFYGameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if(bForceReset&&(Controller!=nullptr))
	{
		Controller->Reset();
	}
	if(APlayerController* PC = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PC,&APlayerController::ServerRestartPlayer_Implementation);
	}
	//TODO BOTController
}


bool AOFYGameMode::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	return true;
}


void AOFYGameMode::FailedToRestartPlayer(AController* NewPlayer)
{
	Super::FailedToRestartPlayer(NewPlayer);
}






