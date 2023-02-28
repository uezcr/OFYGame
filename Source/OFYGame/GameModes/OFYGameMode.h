// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "OFYGameMode.generated.h"


class UOFYPawnData;
class UOFYExperienceDefinition;
class AActor;
class AController;
class AGameModeBase;
class APawn;
class APlayerController;
class UClass;
class UObject;
struct FFrame;
struct FPrimaryAssetId;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGameModeCombinedPostLogin, AGameModeBase* /*GameMode*/, AController* /*NewPlayer*/);
/**
 * 
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class OFYGAME_API AOFYGameMode : public AModularGameModeBase
{
	GENERATED_BODY()
public:
	AOFYGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UOFYPawnData* GetPawnDataForController(const AController* InController) const;
	
	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	virtual void InitGameState() override;
	//~End of AGameModeBase interface

	FOnGameModeCombinedPostLogin& OnGameModeCombinedPostLogin() { return OnGameModeCombinedPostLoginDelegate; }

	// 在下一帧重新启动（重生）指定的玩家或机器人
	// - 如果bForceReset为真，控制器将在这一帧被重置（放弃当前拥有的Pawn，如果有的话）。
	UFUNCTION(BlueprintCallable)
	void RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset = false);

	// PlayerCanRestart的不可知版本，可用于玩家机器人和玩家。
	virtual bool ControllerCanRestart(AController* Controller);

private:
	FOnGameModeCombinedPostLogin OnGameModeCombinedPostLoginDelegate;

protected:
	//~AGameModeBase interface
	virtual bool UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	//~End of AGameModeBase interface

	
	
	void OnExperienceLoaded(const UOFYExperienceDefinition* CurrentExperience);
	bool IsExperienceLoaded() const;

	//根据获取到的ExperienceDefinition的PrimaryAssetID加载ExperienceDefinition
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);
	//获取ExperienceDefinition的PrimaryAssetID
	void HandleMatchAssignmentIfNotExpectingOne();
	
};
