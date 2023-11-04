// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FantasyGameplayAbility.h"
#include "FantasyDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API UFantasyDamageGameplayAbility : public UFantasyGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
