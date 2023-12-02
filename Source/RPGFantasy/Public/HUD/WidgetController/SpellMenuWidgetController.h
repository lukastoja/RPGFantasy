// Copyright Luka Otović

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/FantasyWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSignature, float, NewValue);

UCLASS(BlueprintType, Blueprintable)
class RPGFANTASY_API USpellMenuWidgetController : public UFantasyWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable)
		FOnPlayerStatChangedSignature SpellPointsChanged;
};
