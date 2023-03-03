#include "OFYAssetManagerStartupJob.h"
#include "HAL/Platform.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "OFYLogChannels.h"
#include "Trace/Detail/Channel.h"

TSharedPtr<FStreamableHandle> FOFYAssetManagerStartupJob::DoJob() const
{
	const double JobStartTime = FPlatformTime::Seconds();

	TSharedPtr<FStreamableHandle> Handle;
	UE_LOG(LogOFY, Display, TEXT("Startup job \"%s\" starting"), *JobName);
	JobFunc(*this, Handle);

	if (Handle.IsValid())
	{
		Handle->BindUpdateDelegate(FStreamableUpdateDelegate::CreateRaw(this, &FOFYAssetManagerStartupJob::UpdateSubstepProgressFromStreamable));
		Handle->WaitUntilComplete(0.0f, false);
		Handle->BindUpdateDelegate(FStreamableUpdateDelegate());
	}

	UE_LOG(LogOFY, Display, TEXT("Startup job \"%s\" took %.2f seconds to complete"), *JobName, FPlatformTime::Seconds() - JobStartTime);

	return Handle;
}
