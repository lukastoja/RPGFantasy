// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FantasyDamageGameplayAbility.h"
#include "FantasyProjectileSpell.generated.h"


class AFantasyProjectile;
class UGameplayEffect;
struct FGameplayTag;

UCLASS()
class RPGFANTASY_API UFantasyProjectileSpell : public UFantasyDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
		void SpawnProjectile(const FGameplayTag& SocketTag);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<AFantasyProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
		int32 NumProjectiles = 5;
};
