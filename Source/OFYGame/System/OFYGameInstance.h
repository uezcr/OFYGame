// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonGameInstance.h"
#include "UObject/UObjectGlobals.h"

#include "OFYGameInstance.generated.h"

class UObject;

/**
 * 
 */
UCLASS(Config = Game)
class OFYGAME_API UOFYGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
public:
	UOFYGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//ALyraPlayerController* GetPrimaryPlayerController() const;
	
	virtual bool CanJoinRequestedSession() const override;

protected:
	virtual void Init() override;
	virtual void Shutdown() override;
};
