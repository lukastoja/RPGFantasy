// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/CombatInterface.h"
#include "Characters/CharacterTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributeComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UDebuffNiagaraComponent;
class UPassiveNiagaraComponent;

UCLASS()
class RPGFANTASY_API ABaseCharacter : public ACharacter, public IHitInterface, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:                                              
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;

	UFUNCTION(NetMulticast, Reliable)
		virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual FRotator GetActorRotation_Interface() override;

	virtual TArray<FTaggedMontage> GetAttackMontage_Implementation() override;
	virtual UParticleSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual FOnDeath GetOnDeathDelegate() override;

	virtual USkeletalMeshComponent* GetMeshComponent_Implementation() override;

	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly)
		bool bIsStunned = false;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		float BaseWalkSpeed = 250.f;

	UFUNCTION()
		virtual void OnRep_Stunned();
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

	UPROPERTY(EditAnywhere, Category = "Combat")
		FName LeftHandTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
		FName RightHandTipSocketName;

	UPROPERTY(VisibleAnywhere)
		UAttributeComponent* Atribute;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
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

	bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
		ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;

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

	UPROPERTY(EditAnywhere, Category = "Abilities")
		TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USceneComponent> EffectAttachComponent;

public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
};
