// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributeComponent;

UCLASS()
class RPGFANTASY_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:                                              
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:                                         
	virtual void BeginPlay() override;
	virtual void Attack();

	UFUNCTION(BlueprintNativeEvent)
		void Die();

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

	UPROPERTY(VisibleAnywhere)
		UAttributeComponent* Atribute;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
		double WarpTargetDistance = 75.f;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;

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

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FName> DeathMontageSections;

public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
};
