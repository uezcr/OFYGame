// Fill out your copyright notice in the Description page of Project Settings.


#include "OFYCameraMode.h"

FOFYCameraModeView::FOFYCameraModeView()
	:Location(ForceInit)
    ,Rotation(ForceInit)
    ,ControlRotation(ForceInit)
    ,FieldOfView(80.f)
{
}

void FOFYCameraModeView::Blend(const FOFYCameraModeView& Other, float OtherWeight)
{
}
