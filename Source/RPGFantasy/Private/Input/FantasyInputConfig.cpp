// Copyright Luka Otović


#include "Input/FantasyInputConfig.h"

const UInputAction* UFantasyInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FFantasyInputAction& Action : AbilityInputAction)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
			return Action.InputAction;
	}

	if (bLogNotFound)
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));

	return nullptr;
}
