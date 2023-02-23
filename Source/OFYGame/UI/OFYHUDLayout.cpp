// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OFYHUDLayout.h"
#include "CommonUIExtensions.h"
#include "Delegates/Delegate.h"
#include "Input/CommonUIInputTypes.h"
#include "Misc/AssertionMacros.h"
#include "NativeGameplayTags.h"
#include "UITag.h"
#include "UObject/NameTypes.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYHUDLayout)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");

UOFYHUDLayout::UOFYHUDLayout(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UOFYHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Cyan,TEXT("NativeOnInitialized"));
	RegisterUIActionBinding(FBindUIActionArgs(
		FUIActionTag::ConvertChecked(TAG_UI_ACTION_ESCAPE),
		false,
		FSimpleDelegate::CreateUObject(this, &ThisClass::HandleEscapeAction)));
}

void UOFYHUDLayout::HandleEscapeAction()
{
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Cyan,TEXT("HandleEsc"));
	if(ensure(!EscapeMenuClass.IsNull()))
	{
		UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_MENU, EscapeMenuClass);
	}
}
