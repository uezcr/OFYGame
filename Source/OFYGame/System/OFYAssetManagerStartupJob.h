#pragma once

#include "Containers/UnrealString.h"
#include "Delegates/Delegate.h"
#include "Engine/StreamableManager.h"
#include "HAL/PlatformTime.h"
#include "Templates/Function.h"
#include "Templates/SharedPointer.h"

DECLARE_DELEGATE_OneParam(FOFYAssetManagerStartupJobSubstepProgress, float /*NewProgress*/);

struct FOFYAssetManagerStartupJob
{
	FOFYAssetManagerStartupJobSubstepProgress SubstepProgressDelegate;
	TFunction<void(const FOFYAssetManagerStartupJob&,TSharedPtr<FStreamableHandle>&)> JobFunc;
	FString JobName;
	float JobWeight;
	mutable double LastUpdate = 0;
	FOFYAssetManagerStartupJob(const FString& InJobName,const TFunction<void(const FOFYAssetManagerStartupJob&,TSharedPtr<FStreamableHandle>&)>& InJobFunc,float InJobWeight)
		:JobFunc(InJobFunc)
	    ,JobName(InJobName)
    	,JobWeight(InJobWeight)
	{}
	TSharedPtr<FStreamableHandle> DoJob() const;
	void UpdateSubstepProgress(float NewProgresss) const
	{
		SubstepProgressDelegate.ExecuteIfBound(NewProgresss);
	}

	void UpdateSubstepProgressFromStreamable(TSharedRef<FStreamableHandle> StreamableHandle) const
	{
		if (SubstepProgressDelegate.IsBound())
		{
			// StreamableHandle::GetProgress traverses() a large graph and is quite expensive
			double Now = FPlatformTime::Seconds();
			if (LastUpdate - Now > 1.0 / 60)
			{
				SubstepProgressDelegate.Execute(StreamableHandle->GetProgress());
				LastUpdate = Now;
			}
		}
	}
};
