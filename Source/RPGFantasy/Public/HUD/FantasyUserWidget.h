// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FantasyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGFANTASY_API UFantasyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UObject> WidgetController;

protected:

	UFUNCTION(BlueprintImplementableEvent)
		void WidgetControllerSet();
};
