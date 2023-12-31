// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FantasyGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;

UCLASS()
class RPGFANTASY_API AFantasyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
		TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
		TObjectPtr<UAbilityInfo> AbilityInfo;
};
