// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonActionWidget.h"
#include "OFYActionWidget.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UOFYActionWidget : public UCommonActionWidget
{
	GENERATED_BODY()

public:
	//~ Begin UCommonActionWidget interface
	virtual FSlateBrush GetIcon() const override;
	//~ End of UCommonActionWidget interface

	/** The Enhanced Input Action that is associated with this Common Input action. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	const TObjectPtr<UInputAction> AssociatedInputAction;

private:

	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
	
};
