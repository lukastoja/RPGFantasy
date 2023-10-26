// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FantasyGameplayAbility.h"
#include "FantasyProjectileSpell.generated.h"


class AFantasyProjectile;
class UGameplayEffect;

UCLASS()
class RPGFANTASY_API UFantasyProjectileSpell : public UFantasyGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
		void SpawnProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<AFantasyProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UGameplayEffect> DamageEffectClass;
};
