// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FantasyAnimInstance.h"
#include "Characters/FantasyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UFantasyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	FanatasyCharacter = Cast<AFantasyCharacter>(TryGetPawnOwner());

	if (FanatasyCharacter)
	{
		FanatasyCharacterMovement = FanatasyCharacter->GetCharacterMovement();
	}
}

void UFantasyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (FanatasyCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(FanatasyCharacterMovement->Velocity);
		IsFalling = FanatasyCharacterMovement->IsFalling();
		CharacterState = FanatasyCharacter->GetCharacterState();
		ActionState = FanatasyCharacter->GetActionState();
		DeathPose = FanatasyCharacter->GetDeathPose();
	}
}
