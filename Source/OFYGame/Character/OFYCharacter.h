// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularCharacter.h"
#include "OFYCharacter.generated.h"

class AActor;
class AController;
class AOFYPlayerController;
class UOFYCameraComponent;
class UOFYPawnExtensionComponent;

/**
 * 
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base character pawn class used by this project."))
class OFYGAME_API AOFYCharacter : public AModularCharacter
{
	GENERATED_BODY()
public:
	AOFYCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable,Category= "OFY|Character")
	AOFYPlayerController* GetOFYPlayerController() const;

protected:
	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOFYPawnExtensionComponent> PawnExtComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="OFY|Character",Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOFYCameraComponent> CameraComponent;
	
};
