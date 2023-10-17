// Copyright Luka Otović


#include "HUD/WidgetController/FantasyWidgetController.h"

void UFantasyWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UFantasyWidgetController::BroadcastInitialValues()
{

}

void UFantasyWidgetController::BindCallbacksToDependencies()
{
}
