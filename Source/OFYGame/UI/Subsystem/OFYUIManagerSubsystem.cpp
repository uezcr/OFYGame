// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Subsystem/OFYUIManagerSubsystem.h"

#include "CommonLocalPlayer.h"
#include "Components/SlateWrapperTypes.h"
#include "Containers/Array.h"
#include "Delegates/Delegate.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
//#include "Equipment/LyraEquipmentDefinition.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"
#include "Templates/Casts.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYUIManagerSubsystem)

UOFYUIManagerSubsystem::UOFYUIManagerSubsystem()
{
}

void UOFYUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UOFYUIManagerSubsystem::Tick), 0.0f);
}

void UOFYUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
}

bool UOFYUIManagerSubsystem::Tick(float DeltaTime)
{
	SyncRootLayoutVisibilityToShowHUD();
	return true;
}

void UOFYUIManagerSubsystem::SyncRootLayoutVisibilityToShowHUD()
{
	if(const UGameUIPolicy* Policy = GetCurrentUIPolicy())
	{
		for(const ULocalPlayer* LocalPlayer : GetGameInstance()->GetLocalPlayers())
		{
			bool bShouldShowUI = true;
			if(const APlayerController* PC = LocalPlayer->GetPlayerController(GetWorld()))
			{
				const AHUD* HUD = PC->GetHUD();
				if (HUD && !HUD->bShowHUD)
				{
					bShouldShowUI = false;
				}
			}
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				const ESlateVisibility DesiredVisibility = bShouldShowUI ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
				if (DesiredVisibility != RootLayout->GetVisibility())
				{
					RootLayout->SetVisibility(DesiredVisibility);	
				}
			}
		}
	}
}
