// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Subsystem/OFYUIManagerSubsystem.h"

UOFYUIManagerSubsystem::UOFYUIManagerSubsystem()
{
}

void UOFYUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject())
}

void UOFYUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UOFYUIManagerSubsystem::Tick(float DeltaTime)
{
}

void UOFYUIManagerSubsystem::SyncRootLayoutVisibilityToShowHUD()
{
}
