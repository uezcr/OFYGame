// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "Containers/Array.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "GameplayAbilitySpec.h"
#include "Input/OFYMappableConfigPair.h"
#include "Templates/SubclassOf.h"
#include "UObject/NameTypes.h"
#include "UObject/UObjectGlobals.h"
#include "OFYHeroComponent.generated.h"


class UOFYCameraMode;
class UOFYInputConfig;
struct FInputActionValue;
/**
 * 
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class OFYGAME_API UOFYHeroComponent : public UPawnComponent,public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	UOFYHeroComponent(const FObjectInitializer& ObjectInitializer);
	/** 如果指定的角色上存在Hero组件，则返回该组件。*/
	UFUNCTION(BlueprintPure, Category = "OFY|Hero")
	static UOFYHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UOFYHeroComponent>() : nullptr); }


	/**添加特定模式的输入配置 */
	void AddAdditionalInputConfig(const UOFYInputConfig* InputConfig);

	/** 移除一个特定模式的输入配置，如果它已被添加 */
	void RemoveAdditionalInputConfig(const UOFYInputConfig* InputConfig);

	/** 如果这是一个真正的玩家控制的，并且在初始化过程中取得了足够的进展，可以添加额外的输入绑定，则为 "真"。*/
	bool IsReadyToBindInputs() const;
	
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

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);

	

	TSubclassOf<UOFYCameraMode> DetermineCameraMode() const;

protected:

	UPROPERTY(EditAnywhere)
	TArray<FMappableConfigPair> DefaultInputConfigs;
	
	/** Camera mode set by an ability. */
	UPROPERTY()
	TSubclassOf<UOFYCameraMode> AbilityCameraMode;

	/** True when player input bindings have been applied, will never be true for non - players */
	bool bReadyToBindInputs;
	
};
