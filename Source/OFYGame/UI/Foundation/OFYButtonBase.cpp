// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Foundation/OFYButtonBase.h"
#include "CommonActionWidget.h"

void UOFYButtonBase::SetButtonText(const FText& InText)
{
	bOverride_ButtonText = InText.IsEmpty();
	ButtonText = InText;
	RefreshButtonText();
}

void UOFYButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateButtonStyle();
	RefreshButtonText();
}

void UOFYButtonBase::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();

	
	UpdateButtonStyle();
	RefreshButtonText();
}

void UOFYButtonBase::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);
	UpdateButtonStyle();
}

void UOFYButtonBase::RefreshButtonText()
{
	if (bOverride_ButtonText || ButtonText.IsEmpty())
	{
		if (InputActionWidget)
		{
			const FText ActionDisplayText = InputActionWidget->GetDisplayText();	
			if (!ActionDisplayText.IsEmpty())
			{
				UpdateButtonText(ActionDisplayText);
				return;
			}
		}
	}
	
	UpdateButtonText(ButtonText);	
}