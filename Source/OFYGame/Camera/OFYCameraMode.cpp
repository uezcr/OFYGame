// Fill out your copyright notice in the Description page of Project Settings.
//AllDone

#include "OFYCameraMode.h"

#include "OFYCameraComponent.h"
#include "OFYPlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Canvas.h"
#include "GameFramework/Character.h"
#include "Math/Color.h"
#include "Misc/AssertionMacros.h"
#include "Templates/Casts.h"
#include "UObject/Class.h"
#include "UObject/UnrealNames.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYCameraMode)


FOFYCameraModeView::FOFYCameraModeView()
	:Location(ForceInit)
    ,Rotation(ForceInit)
    ,ControlRotation(ForceInit)
    ,FieldOfView(OFY_CAMERA_DEFAULT_FOV)
{
}

/**
 * FOFYCameraModeView::Blend
 * 关键值：this->Location，(Other->Location，OtherWeight)。
 * 根据OtherWeight值对this->Location进行修改。
 * OtherWeight小于0时不修改。
 * 大于1时直接将this->Location修改为目标值Other->Location。
 * 0和1之间则根据OtherWeight进行插值。
 */
void FOFYCameraModeView::Blend(const FOFYCameraModeView& Other, float OtherWeight)
{
	if(OtherWeight<=0.0f)
	{
		return;
	}
	else if(OtherWeight>=1.0f)
	{
		*this = Other;
		return;
	}
	Location = FMath::Lerp(Location,Other.Location,OtherWeight);
}

/**
 * UOFYCameraMode::UOFYCameraMode
 * 初始化
 */
UOFYCameraMode::UOFYCameraMode()
{
	FieldOfView = OFY_CAMERA_DEFAULT_FOV;
	ViewPitchMin = OFY_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = OFY_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.5f;
	BlendFunction = EOFYCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;
}

/**
 * UOFYCameraMode::GetOFYCameraComponent
 * 返回活跃的OFYCameraComponent
 */
UOFYCameraComponent* UOFYCameraMode::GetOFYCameraComponent() const
{
	return CastChecked<UOFYCameraComponent>(GetOuter());
}

UWorld* UOFYCameraMode::GetWorld() const
{
	return HasAnyFlags(RF_ClassDefaultObject)?nullptr:GetOuter()->GetWorld();
}

/**
 * UOFYCameraMode::GetTargetActor
 * 返回摄像机正在观察的目标角色。
 */
AActor* UOFYCameraMode::GetTargetActor() const
{
	const UOFYCameraComponent* OFYCameraComponent = GetOFYCameraComponent();

	return OFYCameraComponent->GetTargetActor();
}

/**
 * UOFYCameraMode::GetPivotLocation
 * 返回摄像机支点应该处于的位置。
 */
FVector UOFYCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);
	if(const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		//对人物的高度进行调整，以考虑到蹲下的情况。
		if(const ACharacter* TargetCharacter = Cast<ACharacter>(TargetPawn))
		{
			const ACharacter* TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
			check(TargetCharacterCDO);

			const UCapsuleComponent* CapsuleComp = TargetCharacter->GetCapsuleComponent();
			check(CapsuleComp);

			const UCapsuleComponent* CapsuleCompCDO = TargetCharacterCDO->GetCapsuleComponent();
			check(CapsuleCompCDO);

			const float DefaultHalfHeight = CapsuleCompCDO->GetUnscaledCapsuleHalfHeight();
			const float ActualHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
			const float HeightAdjustment = (DefaultHalfHeight - ActualHalfHeight) + TargetCharacterCDO->BaseEyeHeight;

			return TargetCharacter->GetActorLocation()+(FVector::UpVector * HeightAdjustment);
		}
		return TargetPawn->GetPawnViewLocation();
	}
	return TargetActor->GetActorLocation();
}

/**
 * UOFYCameraMode::GetPivotRotation
 * 返回摄像机支点应该处于的旋转。
 */
FRotator UOFYCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if(const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetViewRotation();
	}
	return TargetActor->GetActorRotation();
}

/**
 * UOFYCameraMode::UpdateCameraMode
 * 更新回摄像机的视口和混合。
 */
void UOFYCameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

/**
 * UOFYCameraMode::UpdateView
 * 更新回摄像机的视口。
 */
void UOFYCameraMode::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch,ViewPitchMin,ViewPitchMax);
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void UOFYCameraMode::SetBlendWeight(float Weight)
{
	BlendWeight = FMath::Clamp(Weight, 0.0f, 1.0f);

	//因为我们要直接设置混合权重，所以我们需要计算混合Alpha，以考虑到混合功能。
	const float InvExponent = (BlendExponent > 0.0f) ? (1.0f / BlendExponent) : 1.0f;

	switch (BlendFunction)
	{
	case EOFYCameraModeBlendFunction::Linear:
		BlendAlpha = BlendWeight;
		break;

	case EOFYCameraModeBlendFunction::EaseIn:
		BlendAlpha = FMath::InterpEaseIn(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	case EOFYCameraModeBlendFunction::EaseOut:
		BlendAlpha = FMath::InterpEaseOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	case EOFYCameraModeBlendFunction::EaseInOut:
		BlendAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	default:
		checkf(false, TEXT("SetBlendWeight: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

void UOFYCameraMode::UpdateBlending(float DeltaTime)
{
	if(BlendTime>0.0f)
	{
		BlendTime+=(DeltaTime/BlendTime);
		BlendTime = FMath::Min(BlendAlpha,1.0f);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;

	switch (BlendFunction)
	{
	case EOFYCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;

	case EOFYCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	case EOFYCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	case EOFYCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}


void UOFYCameraMode::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetDrawColor(FColor::White);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("      OFYCameraMode: %s (%f)"), *GetName(), BlendWeight));
}


//////////////////////////////////////////////////////////////////////////
// UOFYCameraModeStack
//////////////////////////////////////////////////////////////////////////

UOFYCameraModeStack::UOFYCameraModeStack()
{
	bIsActive = true;
}

void UOFYCameraModeStack::ActivateStack()
{
	if(!bIsActive)
	{
		bIsActive = true;
		for(UOFYCameraMode* CameraMode:CameraModeStack)
		{
			check(CameraMode);
			CameraMode->OnActivation();
		}
	}
}

void UOFYCameraModeStack::DeactivateStack()
{
	if(bIsActive)
	{
		bIsActive = false;
		for(UOFYCameraMode* CameraMode:CameraModeStack)
		{
			check(CameraMode);
			CameraMode->OnDeactivation();
		}
	}
}

void UOFYCameraModeStack::PushCameraMode(TSubclassOf<UOFYCameraMode> CameraModeClass)
{
	if(!CameraModeClass)
	{
		return;
	}
	UOFYCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);
	int32 StackSize = CameraModeStack.Num();
	if((StackSize>0)&&(CameraModeStack[0]==CameraMode))
	{
		return;
	}
	// 看看它是否已经在堆栈中，并将其删除。
	// 算出它对堆栈的贡献有多大。
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;
	for(int32 StackIndex = 0;StackIndex <StackSize;++StackIndex)
	{
		if(CameraModeStack[StackIndex]==CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->GetBlendWeight();
			break;
		}
		else
		{
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->GetBlendWeight());
		}
	}
	if(ExistingStackIndex!=INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}
	//决定从什么初始权重开始。
	const bool bShouldBlend = ((CameraMode->GetBlendTime()>0.0f&&StackSize>0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);

	CameraMode->SetBlendWeight(BlendWeight);

	//在堆栈顶部添加新条目。
	CameraModeStack.Insert(CameraMode, 0);

	//确保堆栈底层始终是100%的权重。
	CameraModeStack.Last()->SetBlendWeight(1.0f);

	//让相机模式知道它是否被添加到堆栈中。
	if (ExistingStackIndex == INDEX_NONE)
	{
		CameraMode->OnActivation();
	}
}

bool UOFYCameraModeStack::EvaluateStack(float DeltaTime, FOFYCameraModeView& OutCameraModeView)
{
	if(!bIsActive)
	{
		return false;
	}
	UpdateStack(DeltaTime);
	BlendStack(OutCameraModeView);
	return true;
}

UOFYCameraMode* UOFYCameraModeStack::GetCameraModeInstance(TSubclassOf<UOFYCameraMode> CameraModeClass)
{
	check(CameraModeClass);
	for(UOFYCameraMode* CameraMode:CameraModeInstances)
	{
		if((CameraMode!=nullptr)&&(CameraMode->GetClass()==CameraModeClass))
		{
			return CameraMode;
		}
	}
	UOFYCameraMode* NewCameraMode = NewObject<UOFYCameraMode>(GetOuter(),CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	CameraModeInstances.Add(NewCameraMode);
	return NewCameraMode;
}

void UOFYCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if(StackSize<=0)
	{
		return;
	}
	int32 RemoveCount = 0;
	int32 RemoveIndex = 0;
	for(int32 StackIndex = 0;StackIndex<StackSize;++StackIndex)
	{
		UOFYCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);

		if(CameraMode->GetBlendWeight()>=1.0f)
		{
			//这个模式下面的所有东西现在都是不相关的，可以被删除。
			RemoveIndex = (StackIndex+1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}
	if(RemoveCount>0)
	{
		for(int32 StackIndex = RemoveIndex;StackIndex<StackSize;++StackIndex)
		{
			UOFYCameraMode* CameraMode = CameraModeStack[StackIndex];
			check(CameraMode);
			CameraMode->OnDeactivation();
		}
		CameraModeStack.RemoveAt(RemoveIndex,RemoveCount);
	}
}

void UOFYCameraModeStack::BlendStack(FOFYCameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if(StackSize<=0)
	{
		return;
	}

	const UOFYCameraMode* CameraMode = CameraModeStack[StackSize-1];
	check(CameraMode);
	OutCameraModeView = CameraMode->GetCameraModeView();
	for(int32 StackIndex = (StackSize -2);StackIndex>=0;--StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);
		OutCameraModeView.Blend(CameraMode->GetCameraModeView(),CameraMode->GetBlendWeight());
	}
}

void UOFYCameraModeStack::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetDrawColor(FColor::Green);
	DisplayDebugManager.DrawString(FString(TEXT("   --- Camera Modes (Begin) ---")));

	for (const UOFYCameraMode* CameraMode : CameraModeStack)
	{
		check(CameraMode);
		CameraMode->DrawDebug(Canvas);
	}

	DisplayDebugManager.SetDrawColor(FColor::Green);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   --- Camera Modes (End) ---")));
}

void UOFYCameraModeStack::GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const
{
	if (CameraModeStack.Num() == 0)
	{
		OutWeightOfTopLayer = 1.0f;
		OutTagOfTopLayer = FGameplayTag();
		return;
	}
	else
	{
		UOFYCameraMode* TopEntry = CameraModeStack.Last();
		check(TopEntry);
		OutWeightOfTopLayer = TopEntry->GetBlendWeight();
		OutTagOfTopLayer = TopEntry->GetCameraTypeTag();
	}
}





