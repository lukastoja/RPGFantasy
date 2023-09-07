// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FantasyHUD.generated.h"

class UFantasyOverlay;

UCLASS()
class RPGFANTASY_API AFantasyHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Fantasy")
		TSubclassOf<UFantasyOverlay> FantasyOverlayClass;

	UPROPERTY()
		UFantasyOverlay* FantasyOverlay;

public:
	FORCEINLINE UFantasyOverlay* GetFantasyOverlay() const { return FantasyOverlay; }
};
