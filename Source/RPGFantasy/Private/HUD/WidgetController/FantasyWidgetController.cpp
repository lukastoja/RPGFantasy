// Copyright Luka Otović


#include "HUD/WidgetController/FantasyWidgetController.h"
#include "Characters/FantasyPlayerController.h"
#include "Characters/FantasyPlayerState.h"
#include "AbilitySystem/FantasyAbilitySystemComponent.h"
#include "AbilitySystem/FantasyAttributeSet.h"

void UFantasyWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UFantasyWidgetController::BroadcastInitialValues() {}

void UFantasyWidgetController::BindCallbacksToDependencies() {}

void UFantasyWidgetController::BroadcastAbilityInfo()
{
	if (!GetFantasyASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FFantasyAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(FantasyAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = FantasyAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = FantasyAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetFantasyASC()->ForEachAbility(BroadcastDelegate);
}

AFantasyPlayerController* UFantasyWidgetController::GetFantasyPC()
{
	if (FantasyPlayerController == nullptr)
		FantasyPlayerController = Cast<AFantasyPlayerController>(PlayerController);

	return FantasyPlayerController;
}

AFantasyPlayerState* UFantasyWidgetController::GetFantasyPS()
{
	if (FantasyPlayerState == nullptr)
		FantasyPlayerState = Cast<AFantasyPlayerState>(PlayerState);

	return FantasyPlayerState;
}

UFantasyAbilitySystemComponent* UFantasyWidgetController::GetFantasyASC()
{
	if (FantasyAbilitySystemComponent == nullptr)
		FantasyAbilitySystemComponent = Cast<UFantasyAbilitySystemComponent>(AbilitySystemComponent);

	return FantasyAbilitySystemComponent;
}

UFantasyAttributeSet* UFantasyWidgetController::GetFantasyAS()
{
	if (FantasyAttributeSet == nullptr)
		FantasyAttributeSet = Cast<UFantasyAttributeSet>(AttributeSet);

	return FantasyAttributeSet;
}
