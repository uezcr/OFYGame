// Fill out your copyright notice in the Description page of Project Settings.
//TODO GAS

#pragma once

#include "Containers/Array.h"
#include "Templates/SubclassOf.h"
#include "Engine/DataAsset.h"
#include "UObject/UObjectGlobals.h"

#include "OFYPawnData.generated.h"

class APawn;
class UOFYCameraMode;
class UObject;

/**
 * 
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "OFY Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class OFYGAME_API UOFYPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOFYPawnData(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OFY|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Camera")
	TSubclassOf<UOFYCameraMode> DefaultCameraMode;
	
};
