// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "OFYPlayerCameraManager.generated.h"

class FDebugDisplayInfo;
class UCanvas;
class UObject;

#define OFY_CAMERA_DEFAULT_FOV			(80.0f)
#define OFY_CAMERA_DEFAULT_PITCH_MIN	(-89.0f)
#define OFY_CAMERA_DEFAULT_PITCH_MAX	(89.0f)

class UOFYCameraComponent;

/**
 * AOFYPlayerCameraManager
 *
 *	The base player camera manager class used by this project.
 */
UCLASS(notplaceable, MinimalAPI)
class AOFYPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
};
