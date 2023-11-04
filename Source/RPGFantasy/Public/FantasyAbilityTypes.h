#pragma once

#include "GameplayEffectTypes.h"
#include "FantasyAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FFantasyGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	virtual UScriptStruct* GetScriptStruct() const { return StaticStruct(); }

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FFantasyGameplayEffectContext* Duplicate() const
	{
		FFantasyGameplayEffectContext* NewContext = new FFantasyGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
		bool bIsCriticalHit = false;

	UPROPERTY()
		bool bIsBlockedHit = false;
};

template<>
struct TStructOpsTypeTraits<FFantasyGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FFantasyGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};