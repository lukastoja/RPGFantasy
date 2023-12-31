// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "FantasyInputConfig.h"
#include "FantasyInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API UFantasyInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	

public:

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UFantasyInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UFantasyInputComponent::BindAbilityActions(const UFantasyInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FFantasyInputAction& Action : InputConfig->AbilityInputAction)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag);

			if (ReleasedFunc)
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, ReleasedFunc, Action.InputTag);

			if (HeldFunc)
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
		}
	}
}
