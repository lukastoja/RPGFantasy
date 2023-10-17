// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FantasyHUD.generated.h"

class UFantasyOverlay;
class UFantasyUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class RPGFANTASY_API AFantasyHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY()
		TObjectPtr<UFantasyUserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:

	//old way
	UPROPERTY(EditDefaultsOnly, Category = "Fantasy")
		TSubclassOf<UFantasyOverlay> FantasyOverlayClass;

	UPROPERTY()
		UFantasyOverlay* FantasyOverlay;

	//end old way

	//new ver of HUD
	UPROPERTY(EditDefaultsOnly, Category = "Fantasy")
		TSubclassOf<UFantasyUserWidget> OverlayWidgetClass;

	UPROPERTY()
		TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

public:
	FORCEINLINE UFantasyOverlay* GetFantasyOverlay() const { return FantasyOverlay; }
	FORCEINLINE UFantasyUserWidget* GetOverlayWidget() const { return OverlayWidget; }
};
