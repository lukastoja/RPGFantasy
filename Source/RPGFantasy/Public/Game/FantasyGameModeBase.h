// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FantasyGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class UMVVM_LoadSlot;
class USaveGame;
class ULoadScreenSaveGame;

UCLASS()
class RPGFANTASY_API AFantasyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	void TravelToMap(UMVVM_LoadSlot* Slot);

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
		TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
		TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
		FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
		TMap<FString, TSoftObjectPtr<UWorld>> Maps;

protected:

	virtual void BeginPlay() override;
};
