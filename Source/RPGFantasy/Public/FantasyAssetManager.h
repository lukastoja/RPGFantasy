// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "FantasyAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API UFantasyAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UFantasyAssetManager& Get();
	
protected:

	virtual void StartInitialLoading() override;
};
