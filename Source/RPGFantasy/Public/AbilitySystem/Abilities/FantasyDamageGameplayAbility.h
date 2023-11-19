// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FantasyGameplayAbility.h"
#include "Interfaces/CombatInterface.h"
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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UFUNCTION(BlueprintPure)
		FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
