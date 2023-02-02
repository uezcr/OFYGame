// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"

#include "OFYActivatableWidget.generated.h"

UENUM()
enum class EOFYWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

// 一个可激活的小组件，在激活时自动驱动所需的输入配置
UCLASS(Abstract,Blueprintable)
class OFYGAME_API UOFYActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UOFYActivatableWidget(const FObjectInitializer& ObjectInitializer);

public:
	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

protected:
	/** 当这个用户界面被激活时，需要使用的输入模式，例如，你是否希望按键仍能到达游戏/玩家控制器？*/
	UPROPERTY(EditDefaultsOnly,Category = Input)
	EOFYWidgetInputMode InputConfig = EOFYWidgetInputMode::Default;
	/**游戏获得输入时希望的鼠标行为。*/
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
	
};
