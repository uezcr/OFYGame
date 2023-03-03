// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OFYPawnExtensionComponent.h"

#include "OFYGamePlayTags.h"
#include "OFYPawnData.h"
#include "Components/GameFrameworkComponentManager.h"

#include "Net/UnrealNetwork.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(OFYPawnExtensionComponent)

class FLifetimeProperty;
class UActorComponent;

const FName UOFYPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UOFYPawnExtensionComponent::UOFYPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;

	//TODO AbilitySystemComponent = nullptr;
}

void UOFYPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UOFYPawnExtensionComponent, PawnData);
}

void UOFYPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();
	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr), TEXT("OFYPawnExtensionComponent on [%s] can only be added to Pawn actors."), *GetNameSafe(GetOwner()));
	TArray<UActorComponent*> PawnExtensionComponents;
	Pawn->GetComponents(UOFYPawnExtensionComponent::StaticClass(),PawnExtensionComponents);
	ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1), TEXT("Only one OFYPawnExtensionComponent should exist on [%s]."), *GetNameSafe(GetOwner()));
	RegisterInitStateFeature();
}

void UOFYPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
	// Listen for changes to all features
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);
	ensure(TryToChangeInitState(FOFYGameplayTags::Get().InitState_Spawned));
	CheckDefaultInitialization();
}

void UOFYPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UOFYPawnExtensionComponent::SetPawnData(const UOFYPawnData* InPawnData)
{
	check(InPawnData);
	APawn* Pawn = GetPawnChecked<APawn>();

	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}

	PawnData = InPawnData;

	Pawn->ForceNetUpdate();

	CheckDefaultInitialization();
}

void UOFYPawnExtensionComponent::HandleControllerChanged()
{
	CheckDefaultInitialization();
}

void UOFYPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void UOFYPawnExtensionComponent::OnRep_PawnData()
{
	CheckDefaultInitialization();
}

bool UOFYPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FOFYGameplayTags& InitTags = FOFYGameplayTags::Get();
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// As long as we are on a valid pawn, we count as spawned
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// Pawn data is required.
		if (!PawnData)
		{
			return false;
		}

		const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

		if (bHasAuthority || bIsLocallyControlled)
		{
			// Check for being possessed by a controller.
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Transition to initialize if all features have their data available
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}
	else if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UOFYPawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager,
	FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (DesiredState == FOFYGameplayTags::Get().InitState_DataInitialized)
	{
		// This is currently all handled by other components listening to this state change
	}
}

void UOFYPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// If another feature is now in DataAvailable, see if we should transition to DataInitialized
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		const FOFYGameplayTags& InitTags = FOFYGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void UOFYPawnExtensionComponent::CheckDefaultInitialization()
{
	// Before checking our progress, try progressing any other features we might depend on
	CheckDefaultInitializationForImplementers();

	const FOFYGameplayTags& InitTags = FOFYGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}









