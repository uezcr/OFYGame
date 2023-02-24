// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "Delegates/Delegate.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectGlobals.h"
#include "OFYCameraComponent.generated.h"


class UCanvas;
class ULyraCameraMode;
class ULyraCameraModeStack;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FMinimalViewInfo;
template <class TClass> class TSubclassOf;

/**
 * 
 */
UCLASS()
class OFYGAME_API UOFYCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
};
