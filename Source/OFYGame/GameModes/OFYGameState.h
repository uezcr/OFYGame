// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "ModularGameState.h"
#include "UObject/UObjectGlobals.h"
#include "OFYGameState.generated.h"


class APlayerState;
class UOFYExperienceManagerComponent;
class UObject;

/**
 * 添加了ExperienceManager组件的GameState
 */
UCLASS(Config=Game)
class AOFYGameState : public AModularGameStateBase
{
	GENERATED_BODY()

public:
	AOFYGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	//~AGameStateBase interface
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	
	
private:
	UPROPERTY()
	TObjectPtr<UOFYExperienceManagerComponent> ExperienceManagerComponent;
	
};
