// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "OFYHeroComponent.generated.h"


class UOFYCameraMode;
/**
 * 
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class OFYGAME_API UOFYHeroComponent : public UPawnComponent,public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	UOFYHeroComponent(const FObjectInitializer& ObjectInitializer);
	/** Returns the hero component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "OFY|Hero")
	static UOFYHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UOFYHeroComponent>() : nullptr); }

	/** The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
	static const FName NAME_BindInputsNow;

	/** The name of this component-implemented feature */
	static const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	TSubclassOf<UOFYCameraMode> DetermineCameraMode() const;

protected:
	/** Camera mode set by an ability. */
	UPROPERTY()
	TSubclassOf<UOFYCameraMode> AbilityCameraMode;
	
};
