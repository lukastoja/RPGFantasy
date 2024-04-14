// Copyright Luka Otović


#include "Checkpoint/MapEntrance.h"
#include "Interfaces/PlayerInterface.h"
#include "Game/FantasyGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AMapEntrance::LoadActor_Implementation()
{
	// do nothing when loading a map entrance
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

		if (AFantasyGameModeBase* FantasyGM = Cast<AFantasyGameModeBase>(UGameplayStatics::GetGameMode(this)))
			FantasyGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
