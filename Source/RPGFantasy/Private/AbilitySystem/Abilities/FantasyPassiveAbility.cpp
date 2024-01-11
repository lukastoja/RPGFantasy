// Copyright Luka Otović


#include "AbilitySystem/Abilities/FantasyPassiveAbility.h"
#include "AbilitySystem/FantasyAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UFantasyPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UFantasyAbilitySystemComponent* FantasyASC = Cast<UFantasyAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		FantasyASC->DeactivatePassiveAbility.AddUObject(this, &UFantasyPassiveAbility::ReceiveDeactivate);
	}
}

void UFantasyPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
