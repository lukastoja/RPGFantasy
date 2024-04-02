// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FantasyDamageGameplayAbility.h"
#include "FantasyFireBlast.generated.h"

class AFantasyFireBall;

UCLASS()
class RPGFANTASY_API UFantasyFireBlast : public UFantasyDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
		TArray<AFantasyFireBall*> SpawnFireBalls();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
		int32 NumFireBalls = 12;

private:

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AFantasyFireBall> FireBallClass;
	
};
