// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FantasyGameplayAbility.h"
#include "Interfaces/CombatInterface.h"
#include "FantasyAbilityTypes.h"
#include "FantasyDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API UFantasyDamageGameplayAbility : public UFantasyGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		void CauseDamage(AActor* TargetActor); 

	UFUNCTION(BlueprintPure)
		FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DeathImpulseMagnitude = 60.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float KnockbackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float KnockbackChance = 0.f;

	UFUNCTION(BlueprintPure)
		FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
