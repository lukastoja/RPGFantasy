// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "FantasyAbility/FantasyProjectile.h"
#include "FantasyFireBall.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API AFantasyFireBall : public AFantasyProjectile
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
		void StartOutgoingTimeline();

	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite)
		FDamageEffectParams ExplosionDamageParams;

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* Otheractor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
