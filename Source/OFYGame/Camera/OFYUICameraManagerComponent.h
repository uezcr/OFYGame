// Fill out your copyright notice in the Description page of Project Settings.
//AllDone

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "Components/ActorComponent.h"
#include "Misc/AssertionMacros.h"
#include "UObject/UObjectGlobals.h"

#include "OFYUICameraManagerComponent.generated.h"

class AActor;
class AHUD;
class APlayerController;
class FDebugDisplayInfo;
class UCanvas;
class UObject;


UCLASS( Transient, Within=OFYPlayerCameraManager)
class OFYGAME_API UOFYUICameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static UOFYUICameraManagerComponent* GetComponent(APlayerController* PC);
public:
	UOFYUICameraManagerComponent();
	virtual void InitializeComponent() override;
	bool IsSettingViewTarget() const {return bUpdatingViewTarget;}
	AActor* GetViewTarget() const { return ViewTarget; }
	void SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams());

	bool NeedsToUpdateViewTarget() const;
	void UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime);

	void OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos);

private:
	UPROPERTY(Transient)
	TObjectPtr<AActor> ViewTarget;
	
	UPROPERTY(Transient)
	bool bUpdatingViewTarget;
};
