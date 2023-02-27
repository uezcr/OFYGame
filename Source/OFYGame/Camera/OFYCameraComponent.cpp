// Fill out your copyright notice in the Description page of Project Settings.
//AllDone


#include "Camera/OFYCameraComponent.h"

#include "Camera/CameraTypes.h"
#include "Containers/EnumAsByte.h"
#include "Containers/UnrealString.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Scene.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "OFYCameraMode.h"
#include "Math/Color.h"
#include "Math/Rotator.h"
#include "Math/UnrealMathSSE.h"
#include "Math/Vector.h"
#include "Misc/AssertionMacros.h"
#include "Templates/Casts.h"
#include "Templates/SubclassOf.h"
#include "UObject/UObjectBaseUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYCameraComponent)


UOFYCameraComponent::UOFYCameraComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CameraModeStack = nullptr;
	FieldOfViewOffset = 0.0f;
}

void UOFYCameraComponent::OnRegister()
{
	Super::OnRegister();
	if(!CameraModeStack)
	{
		CameraModeStack = NewObject<UOFYCameraModeStack>(this);
		check(CameraModeStack);
	}
}

void UOFYCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);
	
	UpdateCameraModes();
	
	FOFYCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime,CameraModeView);
	if(APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if(APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	CameraModeView.FieldOfView+=FieldOfViewOffset;
	FieldOfViewOffset = 0.0f;

	SetWorldLocationAndRotation(CameraModeView.Location,CameraModeView.Rotation);
	FieldOfView = CameraModeView.FieldOfView;

	// Fill in desired view.
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;

	// See if the CameraActor wants to override the PostProcess settings used.
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
	if (IsXRHeadTrackedCamera())
	{
		// In XR much of the camera behavior above is irrellevant, but the post process settings are not.
		Super::GetCameraView(DeltaTime, DesiredView);
	}
}

void UOFYCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	if (CameraModeStack->IsStackActivate())
	{
		if (DetermineCameraModeDelegate.IsBound())
		{
			if (const TSubclassOf<UOFYCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
			{
				CameraModeStack->PushCameraMode(CameraMode);
			}
		}
	}
}

void UOFYCameraComponent::DrawDebug(UCanvas* Canvas) const
{
	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("LyraCameraComponent: %s"), *GetNameSafe(GetTargetActor())));

	DisplayDebugManager.SetDrawColor(FColor::White);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   Location: %s"), *GetComponentLocation().ToCompactString()));
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   Rotation: %s"), *GetComponentRotation().ToCompactString()));
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   FOV: %f"), FieldOfView));

	check(CameraModeStack);
	CameraModeStack->DrawDebug(Canvas);
}

void UOFYCameraComponent::GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const
{
	check(CameraModeStack);
	CameraModeStack->GetBlendInfo(/*out*/ OutWeightOfTopLayer, /*out*/ OutTagOfTopLayer);
}






