// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/OFYCameraMode.h"
#include "Curves/CurveFloat.h"
#include "OFYPenetrationAvoidanceFeeler.h"
#include "DrawDebugHelpers.h"

#include "OFYCameraMode_ThirdPerson.generated.h"

class UCurveVector;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class UOFYCameraMode_ThirdPerson : public UOFYCameraMode
{
	GENERATED_BODY()
public:
	UOFYCameraMode_ThirdPerson();
protected:
	virtual void UpdateView(float DeltaTime) override;

	void UpdateForTarget(float DeltaTime);
	void UpdatePreventPenetration(float DeltaTime);
	void PreventCameraPenetration(class AActor const& ViewTarget, FVector const& SafeLoc, FVector& CameraLoc, float const& DeltaTime, float& DistBlockedPct, bool bSingleRayOnly);

	virtual void DrawDebug(UCanvas* Canvas) const override;

protected:
	//定义了与目标的局部空间偏移的曲线，使用视距来评估该曲线。
	UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "!bUseRuntimeFloatCurves"))
	TObjectPtr<const UCurveVector> TargetOffsetCurve;

	// UE-103986: 在PIE期间实时编辑RuntimeFloatCurves不起作用（与曲线资产不同）。
	// 一旦这个问题得到解决，这将成为默认的，TargetOffsetCurve将被删除。
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	bool bUseRuntimeFloatCurves;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve TargetOffsetX;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve TargetOffsetY;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve TargetOffsetZ;

	// 改变蹲下的偏移量被混入或混出的速度。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Third Person")
	float CrouchOffsetBlendMultiplier = 5.0f;

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	float PenetrationBlendInTime = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	float PenetrationBlendOutTime = 0.15f;

	/** 如果为真，则进行碰撞检查，使摄像机不在世界范围内。*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	bool bPreventPenetration = true;

	/** 如果为真，尝试检测附近的墙壁，并在预期范围内移动摄像机。 有助于防止弹出。*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	bool bDoPredictiveAvoidance = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionPushOutDistance = 2.f;

	/** 当摄像机的距离由于穿透而被推到其全部距离的这个百分比时 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float ReportPenetrationPercent = 0.f;

	/**
	 * 这些是用来寻找摄像机位置的感应射线。
	 * 索引：0 : 这是我们用来防止碰撞的普通感应器。
	 * 索引：1+ : 如果你的bDoPredictiveAvoidance=true，这些感应器将被用来扫描潜在的撞击，如果玩家
	 * 朝着那个方向旋转，并以原始方式碰撞摄像机，从而使其在撞击阻挡者之前拉入
	 * 撞击遮挡者。
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TArray<FOFYPenetrationAvoidanceFeeler> PenetrationAvoidanceFeelers;

	UPROPERTY(Transient)
	float AimLineToDesiredPosBlockedPct;

	UPROPERTY(Transient)
	TArray<TObjectPtr<const AActor>> DebugActorsHitDuringCameraPenetration;

#if ENABLE_DRAW_DEBUG
	mutable float LastDrawDebugTime = -MAX_FLT;
#endif

protected:
	
	void SetTargetCrouchOffset(FVector NewTargetOffset);
	void UpdateCrouchOffset(float DeltaTime);

	FVector InitialCrouchOffset = FVector::ZeroVector;
	FVector TargetCrouchOffset = FVector::ZeroVector;
	float CrouchOffsetBlendPct = 1.0f;
	FVector CurrentCrouchOffset = FVector::ZeroVector;
	
};
