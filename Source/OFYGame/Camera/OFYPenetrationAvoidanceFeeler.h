#pragma once

#include "CoreMinimal.h"

#include "OFYPenetrationAvoidanceFeeler.generated.h"

/**
 * 定义一个用于避免相机穿透的感应射线的结构。
 */
USTRUCT()
struct FOFYPenetrationAvoidanceFeeler
{
	GENERATED_BODY()

	/**描述偏离主射线的FRotator */
	UPROPERTY(EditAnywhere, Category=PenetrationAvoidanceFeeler)
	FRotator AdjustmentRot;

	/**这个感觉器对最终位置的影响有多大，如果它击中了世界 */
	UPROPERTY(EditAnywhere, Category=PenetrationAvoidanceFeeler)
	float WorldWeight;

	/**如果它撞上了一个Pawn，这个感觉器对最终位置的影响程度（设置为0将完全不尝试与Pawn相撞） */
	UPROPERTY(EditAnywhere, Category=PenetrationAvoidanceFeeler)
	float PawnWeight;

	/**追踪这个感觉器时用于碰撞的范围 */
	UPROPERTY(EditAnywhere, Category=PenetrationAvoidanceFeeler)
	float Extent;

	/**如果上一帧没有击中任何东西，则用该感觉器进行追踪的最小帧间隔 */
	UPROPERTY(EditAnywhere, Category=PenetrationAvoidanceFeeler)
	int32 TraceInterval;

	/**使用该感觉器后的帧数 */
	UPROPERTY(transient)
	int32 FramesUntilNextTrace;


	FOFYPenetrationAvoidanceFeeler()
		: AdjustmentRot(ForceInit)
		, WorldWeight(0)
		, PawnWeight(0)
		, Extent(0)
		, TraceInterval(0)
		, FramesUntilNextTrace(0)
	{
	}

	FOFYPenetrationAvoidanceFeeler(const FRotator& InAdjustmentRot,
									const float& InWorldWeight, 
									const float& InPawnWeight, 
									const float& InExtent, 
									const int32& InTraceInterval = 0, 
									const int32& InFramesUntilNextTrace = 0)
		: AdjustmentRot(InAdjustmentRot)
		, WorldWeight(InWorldWeight)
		, PawnWeight(InPawnWeight)
		, Extent(InExtent)
		, TraceInterval(InTraceInterval)
		, FramesUntilNextTrace(InFramesUntilNextTrace)
	{
	}
};