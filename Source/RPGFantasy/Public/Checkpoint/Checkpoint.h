// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;

UCLASS()
class RPGFANTASY_API ACheckpoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()
	
public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	virtual bool ShouldLoadTransform_Implementation() override { return false; };
	virtual void LoadActor_Implementation() override;

	UPROPERTY(BlueprintReadWrite, SaveGame)
		bool bReached = false;

	UPROPERTY(EditAnywhere, SaveGame)
		bool bBindOverlapCallback = true;

protected:

	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintCallable)
		void HandleGlowEffects();

private:

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USphereComponent> Sphere;
};
