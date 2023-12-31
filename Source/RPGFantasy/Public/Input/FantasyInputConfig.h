// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "FantasyInputConfig.generated.h"


USTRUCT(BlueprintType)
struct FFantasyInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
		FGameplayTag InputTag = FGameplayTag();
};


UCLASS()
class RPGFANTASY_API UFantasyInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FFantasyInputAction> AbilityInputAction;
};
