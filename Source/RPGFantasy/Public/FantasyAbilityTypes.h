#pragma once

#include "GameplayEffectTypes.h"
#include "FantasyAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY(BlueprintReadWrite)
		TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
		TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
		TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
		float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
		float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
		FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
		float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
		float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
		float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
		float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
		float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
		FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
		float KonckbackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
		float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
		FVector KonckbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
		bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
		float RadialDamageInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
		float RadialDamageOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
		FVector RadialDamageOrigin = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FFantasyGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDabuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KonckbackForce; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDabuff = bInIsDebuff; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse; }
	void SetKonckbackForce(const FVector& InForce) { KonckbackForce = InForce; }
	void SetIsRadialDamage(bool InRadialDamage) { bIsRadialDamage = InRadialDamage; }
	void SeRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SeRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }

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

	UPROPERTY()
		bool bIsSuccessfulDabuff = false;

	UPROPERTY()
		float DebuffDamage = 0.f;

	UPROPERTY()
		float DebuffDuration = 0.f;

	UPROPERTY()
		float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
		FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
		FVector KonckbackForce = FVector::ZeroVector;

	UPROPERTY()
		bool bIsRadialDamage = false;

	UPROPERTY()
		float RadialDamageInnerRadius = 0.f;

	UPROPERTY()
		float RadialDamageOuterRadius = 0.f;

	UPROPERTY()
		FVector RadialDamageOrigin = FVector::ZeroVector;
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