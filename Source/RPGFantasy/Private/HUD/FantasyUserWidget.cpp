// Copyright Luka Otović


#include "HUD/FantasyUserWidget.h"

void UFantasyUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
