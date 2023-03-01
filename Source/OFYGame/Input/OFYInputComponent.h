// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "HAL/Platform.h"
#include "InputTriggers.h"
#include "OFYInputConfig.h"
#include "Misc/AssertionMacros.h"
#include "UObject/UObjectGlobals.h"

#include "OFYInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UObject;

UCLASS(Config = Input)
class UOFYInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UOFYInputComponent(const FObjectInitializer& ObjectInitializer);
	
	void AddInputMappings(const UOFYInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
	void RemoveInputMappings(const UOFYInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UOFYInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UOFYInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

	void RemoveBinds(TArray<uint32>& BindHandles);
	
	
};

template <class UserClass, typename FuncType>
void UOFYInputComponent::BindNativeAction(const UOFYInputConfig* InputConfig, const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UOFYInputComponent::BindAbilityActions(const UOFYInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
}
