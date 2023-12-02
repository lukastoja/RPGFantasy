// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "FantasyAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilitySystemComponent;
class USpellMenuWidgetController;
struct FWidgetControllerParams;
class UAbilityInfo;

UCLASS()
class RPGFANTASY_API UFantasyAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "FantasyAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
		static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AFantasyHUD*& OutFantasyHud);

	UFUNCTION(BlueprintPure, Category = "FantasyAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
		static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "FantasyAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
		static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "FantasyAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
		static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "FantasyAbilitySystemLibrary|CharacterClassDefaults", meta = (DefaultToSelf = "WorldContextObject"))
		static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "FantasyAbilitySystemLibrary|CharacterClassDefaults", meta = (DefaultToSelf = "WorldContextObject"))
		static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "FantasyAbilitySystemLibrary|CharacterClassDefaults", meta = (DefaultToSelf = "WorldContextObject"))
		static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "FantasyAbilitySystemLibrary|CharacterClassDefaults", meta = (DefaultToSelf = "WorldContextObject"))
		static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "FantasyAbilitySystemLibrary|GameplayEffects")
		static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "FantasyAbilitySystemLibrary|GameplayEffects")
		static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "FantasyAbilitySystemLibrary|GameplayEffects")
		static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "FantasyAbilitySystemLibrary|GameplayEffects")
		static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "FantasyAbilitySystemLibrary|GameplayMechanics", meta = (DefaultToSelf = "WorldContextObject"))
		static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "FantasyAbilitySystemLibrary|GameplayMechanics")
		static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintPure, Category = "FantasyAbilitySystemLibrary|GameplayMechanics", meta = (DefaultToSelf = "WorldContextObject"))
		static int32 GetXPRewardForClassAndLevel(ECharacterClass CharacterClass, int32 CharacterLevel, const UObject* WorldContextObject);
};
