// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/FantasyHUD.h"
#include "HUD/FantasyOverlay.h"


void AFantasyHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && FantasyOverlayClass)
		{
			FantasyOverlay = CreateWidget<UFantasyOverlay>(Controller, FantasyOverlayClass);
			FantasyOverlay->AddToViewport();
		}
	}
}
