// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "HUD/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;
class UWidgetComponent;

UCLASS()
class RPGFANTASY_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void BeginPlay() override;

	virtual void Die() override;

	void SpawnSoul();
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;

	UPROPERTY(BlueprintReadOnly)
		EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	virtual int32 GetPlayerLevel() override;

	UPROPERTY(BlueprintAssignable)
		FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
		FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		float BaseWalkSpeed = 250.f;

protected:
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
		int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
		ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UWidgetComponent> HealthBar;

private:
	//AI Behavior
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();
	void DecreaseSpawnerCount();

	UFUNCTION()
		void PawnSeen(APawn* SeenPawn);

	UPROPERTY(VisibleAnywhere)
		UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
		double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		double AcceptanceRadius = 150.f;

	//Navigation
	UPROPERTY()
		class AAIController* EnemyController;

	//Current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	//Timers
	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float PatrollingSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float AttackMax = 1.f;	

	UPROPERTY(EditAnywhere, Category = "Combat")
		float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float DeathLifeSpan = 8.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class ASoul> SoulClass;

public:
	FORCEINLINE void SetPatrolTarget(AActor* PatrolTargetNew) { this->PatrolTarget = PatrolTargetNew; };
	FORCEINLINE void SetPatrolTargets(const TArray<AActor*> PatrolTargetsNew) { this->PatrolTargets = PatrolTargetsNew; };
};
