// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FantasyAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
		UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
		AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
		AController* SourceController = nullptr;

	UPROPERTY()
		ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
		UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
		AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
		AController* TargetController = nullptr;

	UPROPERTY()
		ACharacter* TargetCharacter = nullptr;

};

template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;


UCLASS()
class RPGFANTASY_API UFantasyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UFantasyAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>TagsToAttributes;

	//Primary Attributes

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Atrributes")
		FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Atrributes")
		FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Atrributes")
		FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Atrributes")
		FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, Vigor);

	//Secondary attribute

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Atrributes")
		FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Atrributes")
		FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Atrributes")
		FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Atrributes")
		FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Atrributes")
		FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Atrributes")
		FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Atrributes")
		FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Atrributes")
		FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, ManaRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Atrributes")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Atrributes")
		FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, MaxMana);

	//Resistance attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance attributes")
		FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, FireResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightingResistance, Category = "Resistance attributes")
		FGameplayAttributeData LightingResistance;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, LightingResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "Resistance attributes")
		FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, ArcaneResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance attributes")
		FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, PhysicalResistance);

	//Vital Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Atrributes")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Atrributes")
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, Mana);

	//Meta Attributes

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
		FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
		FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UFantasyAttributeSet, IncomingXP);

	UFUNCTION()
		void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
		void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
		void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
		void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
		void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
		void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	//secondary

	UFUNCTION()
		void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
		void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
		void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
		void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
		void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
		void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
		void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
		void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
		void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
		void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	//Resitance rep

	UFUNCTION()
		void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

	UFUNCTION()
		void OnRep_LightingResistance(const FGameplayAttributeData& OldLightingResistance) const;

	UFUNCTION()
		void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;

	UFUNCTION()
		void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;

private:
	void HandleIncomingDamage(const FEffectProperties& Props);
	void HandleIncomingXP(const FEffectProperties& Props);
	void Debuff(const FEffectProperties& Props);
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void SendXPEvent(const FEffectProperties& Props);
	bool bTopOffHealth = false;
	bool bTopOffMana = false;
};
