// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "FantasyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class UFantasyOverlay;
class ASoul;
class ATreasure;

UCLASS()
class RPGFANTASY_API AFantasyCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	AFantasyCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Jump() override;
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Gold) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual int32 GetPlayerLevel() override;
protected:
	virtual void BeginPlay() override;

	//Callback for input
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EKeyPressed();
	virtual void Attack() override;
	void Dodge();

	//Combat
	void Equip(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName);
	virtual void Die_Implementation() override;
	bool HasEnoughStamina();
	bool IsOccupied();

	UFUNCTION(BlueprintCallable)
		void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
		void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
		void FinishEquipping();

	UFUNCTION(BlueprintCallable)
		void HitReactEnd();

private:
	bool IsUnoccupied();
	virtual void InitAbilityActorInfo() override;
	void InitializeFantasyOverlay();
	void SetHUDHealth();

	//Character components
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = "Hair")
		UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = "Hair")
		UGroomComponent* Eyebrows;

	UPROPERTY(VisibleAnywhere)
		AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		UAnimMontage* EquipMontage;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
		UFantasyOverlay* FantasyOverlay;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState;  }
	FORCEINLINE EActionState GetActionState() const { return ActionState; };
};
