// Copyright Luka Otović


#include "AbilitySystem/FantasyAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/FantasyHUD.h"
#include "Characters/FantasyPlayerState.h"
#include "HUD/WidgetController/FantasyWidgetController.h"
#include "HUD/WidgetController/AttributeMenuWidgetController.h"


UOverlayWidgetController* UFantasyAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AFantasyHUD* FantasyHUD = Cast<AFantasyHUD>(PC->GetHUD()))
		{
			AFantasyPlayerState* PS = PC->GetPlayerState<AFantasyPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return FantasyHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UFantasyAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AFantasyHUD* FantasyHUD = Cast<AFantasyHUD>(PC->GetHUD()))
		{
			AFantasyPlayerState* PS = PC->GetPlayerState<AFantasyPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return FantasyHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

