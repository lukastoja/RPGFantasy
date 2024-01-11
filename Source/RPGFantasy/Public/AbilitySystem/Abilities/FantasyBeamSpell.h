// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FantasyDamageGameplayAbility.h"
#include "FantasyBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API UFantasyBeamSpell : public UFantasyDamageGameplayAbility
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		void StoreMouseDataInfo();

	UFUNCTION(BlueprintCallable)
		void StoreOwnerVariables();

	UFUNCTION(BlueprintCallable)
		void TraceFirstTarget(const FVector& BeamTargetLocation);
protected:
	
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
		TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
		TObjectPtr<ACharacter> OwnerCharacter;
};
