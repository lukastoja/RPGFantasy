// Copyright Luka Otović


#include "Characters/FantasyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/FantasyInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/FantasyAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "HUD/Widget/DamageTextComponent.h"
#include "FantasyGameplayTags.h"
#include "FantasyAbility/MagicCircle.h"
#include "Components/DecalComponent.h"

AFantasyPlayerController::AFantasyPlayerController()
{
	bReplicates = true;
}

void AFantasyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	UpdateMagicCircleLocation();
}

void AFantasyPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}

}

void AFantasyPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if(!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AFantasyPlayerController::HideMagicCircle()
{
	if(IsValid(MagicCircle))
		MagicCircle->Destroy();
}

void AFantasyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(FantasyContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
		Subsystem->AddMappingContext(FantasyContext, 0);

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AFantasyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UFantasyInputComponent* FantasyInputComponent = CastChecked<UFantasyInputComponent>(InputComponent);

	FantasyInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFantasyPlayerController::Move);
	FantasyInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AFantasyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	//if (ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	//find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AFantasyPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && !GetASC()->HasMatchingGameplayTag(FFantasyGameplayTags::Get().Player_Block_InputPressed)) 
		GetASC()->AbilityInputTagPressed(InputTag);
}

void AFantasyPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && !GetASC()->HasMatchingGameplayTag(FFantasyGameplayTags::Get().Player_Block_InputReleased))
		GetASC()->AbilityInputTagReleased(InputTag);
}

void AFantasyPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{	
	if (GetASC() && !GetASC()->HasMatchingGameplayTag(FFantasyGameplayTags::Get().Player_Block_InputHeld))
		GetASC()->AbilityInputTagHeld(InputTag);
}

UFantasyAbilitySystemComponent* AFantasyPlayerController::GetASC()
{
	if (FantasyAbilitySystemComponent == nullptr)
		FantasyAbilitySystemComponent = Cast<UFantasyAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));

	return FantasyAbilitySystemComponent;
}

void AFantasyPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			MagicCircle->SetActorLocation(ControlledPawn->GetActorLocation());
		}
	}
}
