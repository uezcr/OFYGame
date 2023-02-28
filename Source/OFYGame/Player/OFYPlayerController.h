// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "OFYPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OFYGAME_API AOFYPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
public:
	AOFYPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
