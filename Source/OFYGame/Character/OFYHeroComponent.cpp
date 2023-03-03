// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OFYHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystems.h"
#include "Player/OFYLocalPlayer.h"
#include "Player/OFYPlayerController.h"
#include "Player/OFYPlayerState.h"
#include "OFYPawnExtensionComponent.h"
#include "OFYGamePlayTags.h"
#include "OFYPawnData.h"
#include "Camera/OFYCameraComponent.h"
#include "Camera/OFYCameraMode.h"
#include "Engine/LocalPlayer.h"
#include "Settings/OFYSettingsLocal.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Input/OFYInputComponent.h"
#include "Input/OFYInputConfig.h"
#include "Misc/UObjectToken.h"
#include "PlayerMappableInputConfig.h"

namespace OFYHero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};


const FName UOFYHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UOFYHeroComponent::NAME_ActorFeatureName("Hero");

UOFYHeroComponent::UOFYHeroComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AbilityCameraMode = nullptr;
}

void UOFYHeroComponent::AddAdditionalInputConfig(const UOFYInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if(!Pawn)
	{
		return;
	}
	UOFYInputComponent* OFYIC = Pawn->FindComponentByClass<UOFYInputComponent>();
	check(OFYIC);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);
	
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	if(const UOFYPawnExtensionComponent* PawnExtensionCom = UOFYPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		//TODO BindAbilityActions()
	}
}

void UOFYHeroComponent::RemoveAdditionalInputConfig(const UOFYInputConfig* InputConfig)
{
	//TODO
}

bool UOFYHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UOFYHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogTemp, Error, TEXT("[UOFYHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("OFYHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("OFYHeroComponent");
			
			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));
				
			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

bool UOFYHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);
	
	const FOFYGameplayTags& InitTags = FOFYGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<AOFYPlayerState>())
		{
			return false;
		}
	
		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();
	
			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);
	
			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}
	
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();
	
		if (bIsLocallyControlled && !bIsBot)
		{
			AOFYPlayerController* LyraPC = GetController<AOFYPlayerController>();
	
			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !LyraPC || !LyraPC->GetLocalPlayer())
			{
				return false;
			}
		}
	
		return true;
	}
	else if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Wait for player state and extension component
		AOFYPlayerState* LyraPS = GetPlayerState<AOFYPlayerState>();
	
		return LyraPS && Manager->HasFeatureReachedInitState(Pawn, UOFYPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}
	else if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}
	
	return false;
}

void UOFYHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	const FOFYGameplayTags& InitTags = FOFYGameplayTags::Get();
	if (CurrentState == FOFYGameplayTags::Get().InitState_DataAvailable && DesiredState == FOFYGameplayTags::Get().InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AOFYPlayerState* OFYPS = GetPlayerState<AOFYPlayerState>();
		if (!ensure(Pawn && OFYPS))
		{
			return;
		}
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UOFYPawnData* PawnData = nullptr;
		
		if (UOFYPawnExtensionComponent* PawnExtComp = UOFYPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UOFYPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			//TODO PawnExtComp->InitializeAbilitySystem(LyraPS->GetLyraAbilitySystemComponent(), LyraPS);
		}

		if (AOFYPlayerController* LyraPC = GetController<AOFYPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
		if (bIsLocallyControlled && PawnData)
		{
			if (UOFYCameraComponent* CameraComponent = UOFYCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void UOFYHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UOFYPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == FOFYGameplayTags::Get().InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UOFYHeroComponent::CheckDefaultInitialization()
{
	const FOFYGameplayTags& InitTags = FOFYGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}



void UOFYHeroComponent::BeginPlay()
{
	Super::BeginPlay();
	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UOFYPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(FOFYGameplayTags::Get().InitState_Spawned));
	CheckDefaultInitialization();
}

void UOFYHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void UOFYHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	const APawn* Pawn = GetPawn<APawn>();
	if(!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const UOFYLocalPlayer* LP = Cast<UOFYLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if(const UOFYPawnExtensionComponent* PawnExtComp = UOFYPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if(const UOFYPawnData* PawnData = PawnExtComp->GetPawnData<UOFYPawnData>())
		{
			if(const UOFYInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FOFYGameplayTags& GameplayTags = FOFYGameplayTags::Get();

				for(const FMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if(Pair.bShouldActivateAutomatically&&Pair.CanBeActivated())
					{
						FModifyContextOptions Options={};
						Options.bIgnoreAllPressedKeysUntilRelease = false;
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(),Options);
					}
				}
				UOFYInputComponent* OFYIC = CastChecked<UOFYInputComponent>(PlayerInputComponent);
				OFYIC->AddInputMappings(InputConfig,Subsystem);
				OFYIC->BindNativeAction(InputConfig,GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
				OFYIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
			}
		}
	}
	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UOFYHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController():nullptr;
	if(AOFYPlayerController* OFYController = Cast<AOFYPlayerController>(Controller))
	{
		//TODO AutoRun
	}
	if(Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f,Controller->GetControlRotation().Yaw,0.0f);
		
		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UOFYHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UOFYHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * OFYHero::LookYawRate * World->GetDeltaSeconds());
	}
	
	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * OFYHero::LookPitchRate * World->GetDeltaSeconds());
	}
}

TSubclassOf<UOFYCameraMode> UOFYHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UOFYPawnExtensionComponent* PawnExtComp = UOFYPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UOFYPawnData* PawnData = PawnExtComp->GetPawnData<UOFYPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
