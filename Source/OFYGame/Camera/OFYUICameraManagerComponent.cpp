// Fill out your copyright notice in the Description page of Project Settings.


#include "OFYUICameraManagerComponent.h"

#include "OFYPlayerCameraManager.h"
#include "GameFramework/HUD.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYUICameraManagerComponent)

class AActor;
class FDebugDisplayInfo;

UOFYUICameraManagerComponent* UOFYUICameraManagerComponent::GetComponent(APlayerController* PC)
{
	if(PC!=nullptr)
	{
		if(AOFYPlayerCameraManager* PCCamera = Cast<AOFYPlayerCameraManager>(PC->PlayerCameraManager))
		{
			return PCCamera->GetUICameraComponent();
		}
	}
	return nullptr;
}

UOFYUICameraManagerComponent::UOFYUICameraManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	if(!HasAnyFlags(RF_ClassDefaultObject))
	{
		if(!IsRunningDedicatedServer())
		{
			AHUD::OnShowDebugInfo.AddUObject(this,&ThisClass::OnShowDebugInfo);
		}
	}
}

void UOFYUICameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UOFYUICameraManagerComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
	TGuardValue<bool> UpdatingViewTargetGuard(bUpdatingViewTarget, true);

	ViewTarget = InViewTarget;
	CastChecked<AOFYPlayerCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool UOFYUICameraManagerComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void UOFYUICameraManagerComponent::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
}

void UOFYUICameraManagerComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo,
	float& YL, float& YPos)
{
}


