// Copyright Luka Otović

#include "AbilitySystem/Abilities/FantasyProjectileSpell.h"
#include "FantasyAbility/FantasyProjectile.h"
#include "Interfaces/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FantasyGameplayTags.h"

void UFantasyProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UFantasyProjectileSpell::SpawnProjectile(const FGameplayTag& SocketTag)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag));

		FRotator Rotation = CombatInterface->GetActorRotation_Interface();
		Rotation.Pitch = 0.f;
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AFantasyProjectile* Projectile = GetWorld()->SpawnActorDeferred<AFantasyProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Projectile->FinishSpawning(SpawnTransform);
	}
}
