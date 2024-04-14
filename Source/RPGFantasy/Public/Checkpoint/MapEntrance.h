// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
		FName DestinationPlayerStartTag = FName();

	virtual void LoadActor_Implementation() override;

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
