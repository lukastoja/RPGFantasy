// Copyright Luka Otović


#include "AbilitySystem/FantasyAbilitySystemGlobals.h"
#include "FantasyAbilityTypes.h"

FGameplayEffectContext* UFantasyAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FFantasyGameplayEffectContext();
}
