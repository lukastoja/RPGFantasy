// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "FantasyAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API UFantasyAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
