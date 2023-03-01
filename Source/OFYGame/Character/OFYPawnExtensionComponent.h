// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "OFYPawnExtensionComponent.generated.h"

class UOFYPawnData;
/**
 * 为所有APawn类添加功能的组件，因此它可以用于角色/车辆等。
 * 这协调了其他组件的初始化。
 */
UCLASS()
class OFYGAME_API UOFYPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UOFYPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	/** 这个整体特征的名称，这个特征取决于其他命名的组件特征。 */
	static const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

	/** 如果指定的角色上存在APawn的扩展组件，则返回该组件。*/
	UFUNCTION(BlueprintPure, Category = "OFY|Pawn")
	static UOFYPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UOFYPawnExtensionComponent>() : nullptr); }

	/** 获取Pawn的数据，用于在数据中指定Pawn的属性。 */
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	/** Sets the current pawn data */
	void SetPawnData(const UOFYPawnData* InPawnData);

	/** Should be called by the owning pawn when the pawn's controller changes. */
	void HandleControllerChanged();

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
    void OnRep_PawnData();

	/** Pawn data used to create the pawn. Specified from a spawn function or on a placed instance. */
	UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category = "Lyra|Pawn")
	TObjectPtr<const UOFYPawnData> PawnData;
	
};
