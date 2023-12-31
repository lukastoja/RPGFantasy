// Copyright Luka Otović


#include "AbilitySystem/Abilities/FantasyFireBolt.h"

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
			FMath::Min(Level, NumProjectiles),
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
		FMath::Min(Level, NumProjectiles),
		ScaledDamage
	);
}