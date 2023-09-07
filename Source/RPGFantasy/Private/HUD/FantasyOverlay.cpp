// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/FantasyOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UFantasyOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar) HealthProgressBar->SetPercent(Percent);
}

void UFantasyOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar) StaminaProgressBar->SetPercent(Percent);
}

void UFantasyOverlay::SetGold(int32 Gold)
{
	if (GoldText) GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
}

void UFantasyOverlay::SetSouls(int32 Souls)
{
	if (SoulsText) SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
}
