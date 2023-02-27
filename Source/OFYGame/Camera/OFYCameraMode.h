// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "GameplayTagContainer.h"
#include "Math/Rotator.h"
#include "Math/UnrealMathSSE.h"
#include "Math/Vector.h"
#include "Templates/SubclassOf.h"
#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "OFYCameraMode.generated.h"

class AActor;
class UCanvas;
class UOFYCameraComponent;

/**
 * EOFYCameraModeBlendFunction
 *	用于相机模式之间转换的混合函数
 */
UENUM(BlueprintType)
enum class EOFYCameraModeBlendFunction : uint8
{
	// 做一个简单的线性插值
	Linear,

	// 立即加速,但平稳地减速进入目标,缓和量由指数控制
	EaseIn,

	// 平稳地加速,但不减速进入目标,缓和量由指数控制
	EaseOut,

	// 平稳地加速和减速,缓和量由指数控制
	EaseInOut,

	COUNT	UMETA(Hidden)
};

/**
 * FOFYCameraModeView
 *
 *	由相机模式产生的视图数据,用于混合相机模式.
 */
struct FOFYCameraModeView
{
public:

	//初始化
	FOFYCameraModeView();

	//混合
	void Blend(const FOFYCameraModeView& Other, float OtherWeight);

public:

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

/**
 * UOFYCameraMode
 *
 *	Base class for all camera modes.
 */
UCLASS(Abstract, NotBlueprintable)
class OFYGAME_API UOFYCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UOFYCameraMode();

	UOFYCameraComponent* GetOFYCameraComponent() const;

	virtual UWorld* GetWorld() const override;

	AActor* GetTargetActor() const;

	const FOFYCameraModeView& GetCameraModeView() const {return View;}
		// Called when this camera mode is activated on the camera mode stack.
	virtual void OnActivation() {};

	// Called when this camera mode is deactivated on the camera mode stack.
	virtual void OnDeactivation() {};

	void UpdateCameraMode(float DeltaTime);

	float GetBlendTime() const { return BlendTime; }
	float GetBlendWeight() const { return BlendWeight; }
	void SetBlendWeight(float Weight);

	FGameplayTag GetCameraTypeTag() const
	{
		return CameraTypeTag;
	}

	virtual void DrawDebug(UCanvas* Canvas) const;

protected:

	virtual FVector GetPivotLocation() const;
	virtual FRotator GetPivotRotation() const;

	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);

protected:
	// 一个可以被游戏代码查询的标签，它关心的是某种相机模式是否处于活动状态。
	// 而不需要询问特定的模式（例如，当下视瞄准以获得更高的精确度时）。
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	FGameplayTag CameraTypeTag;

	//查看由相机模式产生的输出。
	FOFYCameraModeView View;

	// 水平视场（单位：度）。
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	//最小视距（单位：度）。
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	//最大视距（单位：度）。
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	//在这种模式下需要多长时间来混合。
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	//用于混合的函数。
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	EOFYCameraModeBlendFunction BlendFunction;

	//混合函数使用的指数，用于控制曲线的形状。
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	//用于确定混合权重的线性混合Alpha。
	float BlendAlpha;

	//使用混合alpha和函数计算的混合权重。
	float BlendWeight;

protected:
	/** 如果为真，则跳过所有插值，将摄像机置于理想位置。 下一帧自动设置为假。*/
	UPROPERTY(transient)
	uint32 bResetInterpolation:1;
	
};

/**
 * UOFYCameraModeStack
 *
 *用于混合相机模式的堆栈。
 */
UCLASS()
class UOFYCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UOFYCameraModeStack();
	void ActivateStack();
	void DeactivateStack();

	bool IsStackActivate() const { return bIsActive; }

	void PushCameraMode(TSubclassOf<UOFYCameraMode> CameraModeClass);

	bool EvaluateStack(float DeltaTime, FOFYCameraModeView& OutCameraModeView);

	void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;
protected:

	UOFYCameraMode* GetCameraModeInstance(TSubclassOf<UOFYCameraMode> CameraModeClass);

	void UpdateStack(float DeltaTime);
	void BlendStack(FOFYCameraModeView& OutCameraModeView) const;

protected:

	bool bIsActive;

	UPROPERTY()
	TArray<TObjectPtr<UOFYCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UOFYCameraMode>> CameraModeStack;
};


