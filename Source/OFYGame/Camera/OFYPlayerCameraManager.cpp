// Fill out your copyright notice in the Description page of Project Settings.
//AllDone


#include "Camera/OFYPlayerCameraManager.h"

#include "OFYCameraComponent.h"
#include "OFYUICameraManagerComponent.h"
#include "Engine/Canvas.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYPlayerCameraManager)
class FDebugDisplayInfo;
static FName UICameraComponentName(TEXT("UICamera"));

AOFYPlayerCameraManager::AOFYPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	DefaultFOV = OFY_CAMERA_DEFAULT_FOV;
	ViewPitchMin = OFY_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = OFY_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<UOFYUICameraManagerComponent>(UICameraComponentName);
}

UOFYUICameraManagerComponent* AOFYPlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void AOFYPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	if(UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT,DeltaTime);
		return;
	}
	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void AOFYPlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL,
	float& YPos)
{
	check(Canvas);
	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;
	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("LyraPlayerCameraManager: %s"), *GetNameSafe(this)));
	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const UOFYCameraComponent* CameraComponent = UOFYCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}
