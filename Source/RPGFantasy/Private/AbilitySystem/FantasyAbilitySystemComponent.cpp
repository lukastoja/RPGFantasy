// Copyright Luka Otović


#include "AbilitySystem/FantasyAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/FantasyGameplayAbility.h"
#include "FantasyGameplayTags.h"

void UFantasyAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UFantasyAbilitySystemComponent::ClientEffectApplied);
}

void UFantasyAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UFantasyGameplayAbility* FantasyAbility = Cast<UFantasyGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(FantasyAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UFantasyAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && !AbilitySpec.IsActive())
		{
			AbilitySpecInputPressed(AbilitySpec);
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UFantasyAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			AbilitySpecInputReleased(AbilitySpec);
	}
}

void UFantasyAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
