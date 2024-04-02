// Copyright Luka Otović


#include "AbilitySystem/Abilities/FantasyFireBolt.h"
#include "FantasyAbility/FantasyProjectile.h"
#include "FantasyGameplayTags.h"

FString UFantasyFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches a bolt of fire, exploding on impact and dealing: </>"
			"<Damage>%d</><Default> "
			"fire damage with chance to burn</>"
			),
			Level, 
			ManaCost,
			Cooldown,
			ScaledDamage
			);
	}
	else
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
			"<Damage>%d</><Default> "
			"fire damage with chance to burn</>"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumProjectiles),
			ScaledDamage
		);
}

FString UFantasyFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL:</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
		"<Damage>%d</><Default> "
		"fire damage with chance to burn</>"
		),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumProjectiles),
		ScaledDamage
	);
}

void UFantasyFireBolt::SpawnProjectiles(const FGameplayTag& SocketTag)
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

		const int32 NumProjectiles = FMath::Min(GetAbilityLevel(), MaxNumProjectiles);
		const float AngleBetweenProjectiles = 360.f / NumProjectiles;

		for (int32 i = 0; i < FMath::Min(GetAbilityLevel(), NumProjectiles); i++)
		{
			// Koristite trigonometriju da biste odredili položaj projektila na kružnici
			if (i > 0)
			{
				float AngleInRadians = FMath::DegreesToRadians(AngleBetweenProjectiles * i);
				SpawnTransform.SetLocation(SpawnTransform.GetLocation() + FVector(FMath::Cos(AngleInRadians) * CircleRadius, FMath::Sin(AngleInRadians) * CircleRadius, 0.f));
			}


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
}
