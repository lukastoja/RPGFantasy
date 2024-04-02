// Copyright Luka Otović


#include "Actor/FantasyFireBall.h"
#include <Interfaces/CombatInterface.h>
#include <AbilitySystemBlueprintLibrary.h>
#include <AbilitySystem/FantasyAbilitySystemLibrary.h>

void AFantasyFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AFantasyFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* Otheractor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(Otheractor)) return;

	if (HasAuthority())
	{
		//tu provijeriti da li otheractor ima icombat interface i izračunat od kuda je došao hit i to postaviti u var
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Otheractor))
		{
			CombatInterface->CalculateHitReactMontage(GetActorLocation());
		}

		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Otheractor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;

			if (bKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;

				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KonckbackForceMagnitude;
				DamageEffectParams.KonckbackForce = KnockbackForce;
			}

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UFantasyAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}

		Destroy();
	}
	else bHit = true;
}
