// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/FantasyHUD.h"
#include "HUD/FantasyUserWidget.h"
#include "HUD/FantasyOverlay.h"
#include "HUD/WidgetController/OverlayWidgetController.h"
#include "HUD/WidgetController/AttributeMenuWidgetController.h"


UOverlayWidgetController* AFantasyHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AFantasyHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}

	return AttributeMenuWidgetController;
}

void AFantasyHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget controller class unitialized, please fill out BP_FantasyHUD"));
	checkf(OverlayWidgetClass, TEXT("Overlay widget class unitialized, please fill out BP_FantasyHUD"));

	UWorld* World = GetWorld();
	if (World)
	{
		const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
		UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

		OverlayWidget = CreateWidget<UFantasyUserWidget>(World, OverlayWidgetClass);
		OverlayWidget->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitialValues();
		OverlayWidget->AddToViewport();
	}
}
