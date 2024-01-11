// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FantasyGameplayAbility.h"
#include "FantasyPassiveAbility.generated.h"


UCLASS()
class RPGFANTASY_API UFantasyPassiveAbility : public UFantasyGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ReceiveDeactivate(const FGameplayTag& AbilityTag);

protected:

private:
	
};
