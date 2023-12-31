// Copyright Luka Otović


#include "FantasyAssetManager.h"
#include "FantasyGameplayTags.h"

UFantasyAssetManager& UFantasyAssetManager::Get()
{
	check(GEngine);

	UFantasyAssetManager* FantasyAssetManager = Cast<UFantasyAssetManager>(GEngine->AssetManager);
	return *FantasyAssetManager;
}

void UFantasyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FFantasyGameplayTags::InitializeNativeGameplayTags();
}