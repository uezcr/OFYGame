// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OFYPlayerController.h"

#include "Camera/OFYPlayerCameraManager.h"

AOFYPlayerController::AOFYPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AOFYPlayerCameraManager::StaticClass();
}
