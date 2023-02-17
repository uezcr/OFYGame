// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/OFYActivatableWidget.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "OFYHUDLayout.generated.h"

class UCommonActivatableWidget;
class UObject;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "OFY HUD Layout", Category = "OFY|HUD"))
class OFYGAME_API UOFYHUDLayout : public UOFYActivatableWidget
{
	GENERATED_BODY()

public:
	UOFYHUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
	
};
