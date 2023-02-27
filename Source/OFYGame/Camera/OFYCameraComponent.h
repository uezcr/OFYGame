// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "Delegates/Delegate.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectGlobals.h"
#include "OFYCameraComponent.generated.h"


class UCanvas;
class UOFYCameraMode;
class UOFYCameraModeStack;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FMinimalViewInfo;
template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UOFYCameraMode>, FOFYCameraModeDelegate);

/**
 * 
 */
UCLASS()
class OFYGAME_API UOFYCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	UOFYCameraComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure,Category="OFY|Camera")
	static UOFYCameraComponent* FindCameraComponent(const AActor* Actor) {return (Actor ? Actor->FindComponentByClass<UOFYCameraComponent>() : nullptr); }

	virtual AActor* GetTargetActor() const {return GetOwner();}

	FOFYCameraModeDelegate DetermineCameraModeDelegate;

	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }

	virtual void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:
	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void UpdateCameraModes();

protected:

	// Stack used to blend the camera modes.
	UPROPERTY()
	TObjectPtr<UOFYCameraModeStack> CameraModeStack;

	// Offset applied to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	float FieldOfViewOffset;
	
};
