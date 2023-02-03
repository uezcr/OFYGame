// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/HUD.h"
#include "UObject/UObjectGlobals.h"

#include "OFYHUD.generated.h"

class AActor;
class UObject;

UCLASS(Config = Game)
class AOFYHUD : public AHUD
{
	GENERATED_BODY()

public:
	AOFYHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	//~UObject interface
	virtual void PreInitializeComponents() override;
	//~End of UObject interface

	//~AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	//~AHUD interface
	virtual void GetDebugActorList(TArray<AActor*>& InOutList) override;
	//~End of AHUD interface
};
