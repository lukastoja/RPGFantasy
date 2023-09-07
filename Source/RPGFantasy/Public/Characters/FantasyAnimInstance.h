// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "FantasyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API UFantasyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
		class AFantasyCharacter* FanatasyCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		class UCharacterMovementComponent* FanatasyCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float GroundSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		TEnumAsByte<EDeathPose> DeathPose;
};
