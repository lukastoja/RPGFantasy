// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FantasyAbilityTypes.h"
#include "FantasyProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class RPGFANTASY_API AFantasyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AFantasyProjectile();

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		FDamageEffectParams DamageEffectParams;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* Otheractor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Destroyed() override;
	void OnHit();
private:

	UPROPERTY(EditDefaultsOnly)
		float LifeSpan = 15.f;	

	bool bHit = false;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY()
		TObjectPtr<UAudioComponent> LoopingSoundComponent;
};
