// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "OFYPlayerState.generated.h"


class UOFYPawnData;
/**
 * 
 */
UCLASS(Config = Game)
class OFYGAME_API AOFYPlayerState : public AModularPlayerState
{
	GENERATED_BODY()

public:
	AOFYPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	template <class T>
    const T* GetPawnData() const { return Cast<T>(PawnData); }


protected:
	UFUNCTION()
	void OnRep_PawnData();
protected:
	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UOFYPawnData> PawnData;
	
};
