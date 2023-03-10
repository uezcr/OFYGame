// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OFYActivatableWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYActivatableWidget)

#define LOCTEXT_NAMESPACE "OFY"

UOFYActivatableWidget::UOFYActivatableWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UOFYActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EOFYWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All,GameMouseCaptureMode);
	case EOFYWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game,GameMouseCaptureMode);
	case EOFYWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu,EMouseCaptureMode::NoCapture);
	case EOFYWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
