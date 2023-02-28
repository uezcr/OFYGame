// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OFYCameraAssistInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UOFYCameraAssistInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OFYGAME_API IOFYCameraAssistInterface
{
	GENERATED_BODY()
public:

	/**
	 * 获取我们允许摄像机穿透的演员列表。在第三人称摄像机中很有用
	 * 当你需要下面的摄像机忽略一些东西时，比如一个视图目标集合，Pawn。
	 * 一个车辆...等等。
	 */
	virtual void GetIgnoredActorsForCameraPentration(TArray<const AActor*>& OutActorsAllowPenetration) const { }

	/**
	 * 要防止穿透的目标角色。 通常情况下，这几乎总是视图目标，如果
	 * 未实现的情况下将保持为真。 然而，有时视图目标并不等同于根行为体 
	 * 你需要保持在框架内。
	 */
	virtual TOptional<AActor*> GetCameraPreventPenetrationTarget() const
	{
		return TOptional<AActor*>();
	}

	/** 如果摄像机穿透了焦点目标，就会被调用。 如果你想在被重叠的时候隐藏目标对象，这很有用。*/
	virtual void OnCameraPenetratingTarget() { }
};
