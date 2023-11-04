// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/CombatInterface.h"
#include "Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributeComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

UCLASS()
class RPGFANTASY_API ABaseCharacter : public ACharacter, public IHitInterface, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:                                              
	ABaseCharacter();
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;

	UFUNCTION(NetMulticast, Reliable)
		virtual void MulticastHandleDeath();
protected:                                         
	virtual void BeginPlay() override;
	virtual void Attack();

	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	virtual bool CanAttack();
	bool IsAlive();
	void DisableMeshCollision();
	void PlayHitReactMontage(const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	virtual void PlayDodgeMontage();
	void StopAttackMontage();

	virtual void InitAbilityActorInfo();

	UFUNCTION(BlueprintCallable)
		FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
		FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
		virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
		virtual void DodgeEnd();

	UFUNCTION(BlueprintCallable)
		void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UPROPERTY(VisibleAnywhere)
		AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
		FName WeaponTipSocketName;

	virtual FVector GetCombatSocketLocation() override;

	virtual FRotator GetActorRotation_Interface() override;

	UPROPERTY(VisibleAnywhere)
		UAttributeComponent* Atribute;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
		double WarpTargetDistance = 75.f;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;

	//GAS

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
		TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
		TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
		TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
		TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	
	void AddCharacterAbilities();

private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
		UParticleSystem* HitParticles;

	//Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FName> DeathMontageSections;

	UPROPERTY(EditAnywhere, Category = "Abilities")
		TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
};
