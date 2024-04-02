// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FantasyDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

/**
 *
 */
UCLASS()
class RPGFANTASY_API UArcaneShards : public UFantasyDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumShards = 11;
};
