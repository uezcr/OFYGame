// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Foundation/OFYActionWidget.h"

#include "CommonInputSubsystem.h"
#include "EnhancedInputSubsystems.h"

FSlateBrush UOFYActionWidget::GetIcon() const
{
	// 如果有一个与此部件相关的增强型输入动作，那么就搜索与该动作绑定的任何
	// 绑定到该动作的任何键，并显示这些键而不是默认的数据表设置。
	// 这包括了当玩家将一个键绑定到其他东西上的情况。
	if (AssociatedInputAction)
	{
		UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = GetEnhancedInputSubsystem();
		TArray<FKey> BoundKeys = EnhancedInputSubsystem->QueryKeysMappedToAction(AssociatedInputAction);
		FSlateBrush SlateBrush;

		if (!BoundKeys.IsEmpty() && UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, BoundKeys[0], CommonInputSubsystem->GetCurrentInputType(), CommonInputSubsystem->GetCurrentGamepadName()))
		{
			return SlateBrush;
		}
	}
	
	return Super::GetIcon();
}

UEnhancedInputLocalPlayerSubsystem* UOFYActionWidget::GetEnhancedInputSubsystem() const
{
	const UWidget* BoundWidget = DisplayedBindingHandle.GetBoundWidget();
	const ULocalPlayer* BindingOwner = BoundWidget ? BoundWidget->GetOwningLocalPlayer() : GetOwningLocalPlayer();
	
	return BindingOwner->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}
