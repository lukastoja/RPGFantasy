// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FantasyProjectileSpell.h"
#include "FantasyFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API UFantasyFireBolt : public UFantasyProjectileSpell
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
